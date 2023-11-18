/* This file contains definitions for the PacketTableView class (derived from QTableView)
 * Ramsey Nofal, 08/2023
 */


//Standard Qt
#include <QtConcurrent>
#include <QModelIndex>

//Local
#include "PacketTableView.h"
#include "util.h"


PacketTableView::PacketTableView(QWidget *parent):
                                QTableView(parent),
                                StyleWidget(this),
                                packet_update_enabled(true),
                                ui_closed(false),
                                num_packets_total(0),
                                num_packets_to_display(0){
    model = new QStandardItemModel();
    this->setModel(model);

    config_model();
    config_mode_options();

    connect(this, SIGNAL(clicked(QModelIndex)), this, SLOT(clicked_packet(QModelIndex)));
}

void PacketTableView::config_model() {
    int index = 0;
    for(auto& header : model_col_headers) {
        model->setHorizontalHeaderItem(index++, new QStandardItem(header));
    }
}

void PacketTableView::config_mode_options() {
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void PacketTableView::add_row(const std::vector<QString>& data) {
    int in_data_size = (int) data.size();
    int model_num_cols = (int) model_col_headers.size();

    //We don't want data amount mismatch to be a blocking bug
    if(in_data_size < model_num_cols) {
        qDebug() << "Too few input data in PacketTableView::add_row()";
    } else if(in_data_size > model_num_cols) {
        qDebug() << "Too many input data in PacketTableView::add_row()";
    }

    QList<QStandardItem*> new_row;
    for(int i = 0; i < model_num_cols && i < in_data_size; ++i) {
        auto entry = data[i];
        new_row.append(new QStandardItem(entry));
    }
    model->appendRow(new_row);
}

void PacketTableView::clear_view() {
    model->clear();

    tcp_indices.clear();
    udp_indices.clear();
    icmp_indices.clear();
    ip_indices.clear();

    packets.clear();

    num_packets_total = 0;
    num_packets_to_display = 0;

    //table_to_real_packet_index.clear();

    config_model();
}

//void PacketTableView::add_packet_index(const int& num) {
//    table_to_real_packet_index.push_back(num);

//}
//int PacketTableView::get_packet_num_from_row(const int& row) {
//    //We want this to be non-blocking
//    if(row >= table_to_real_packet_index.size()) {
//        qDebug() << "Invalid row or incorrect mapping in PacketTableView::get_packet_index_from_row()";
//        return 0;
//    }

//    return table_to_real_packet_index[row];
//}

/* Note: packet_queue and poll_queue_thread could/should be set in the PacketTableView constructor.
 * However, QtCreator auto-generates the construction of the PacketTableView within ui_mainwindow.h, limiting
 * what data we can pass into the constructor.
 *
 * Solution 1 (non-ideal): move PacketTableView construction into a user-created file (not auto-gen). Downside: makes
 * UI object construction less consistent.
 *
 * Solution 2 (ideal?): move all UI object construction into a user-created file(s). Downside: reduces/removes the
 * utility of qtcreator's UI editor.
 *
 * Solution 3 (current): set these variables after construction, with some guards to limit any ill after effects. Downside:
 * guards/checks may be expensive, somewhat confusing.
 */
void PacketTableView::set_packet_queue(SharedQueue<Packet*> *in_packet_queue) {
    packet_queue = in_packet_queue;
    qDebug() << "packet_queue set in PacketTableView::set_packet_queue()";
}
void PacketTableView::start_queue_poll() {
    #ifdef QT_DEBUG
    if(packet_queue == nullptr) {
        throw  std::runtime_error("packet_queue in PacketTableView is nullptr. Call set_packet_queue() before use");
    }
    #endif
    poll_queue_thread = QtConcurrent::run([this] {poll_queue();});
    ui_update_thread = QtConcurrent::run([this] {update_ui_loop();});
    qDebug() << "packet_queue poll started in PacketTableView::start_queue_poll()";
}

void PacketTableView::poll_queue() {
    while(!ui_closed) {
        if(packet_update_enabled && !packet_queue->is_empty()) {
            add_packet(packet_queue->pop());
        }
    }
    qDebug() << "PacketTableView::poll_queue() halted";
}

void PacketTableView::add_packet(Packet* new_packet) {
    //Add to model
    add_row({new_packet->get_type_name(),
             new_packet->get_src_ip() + ":" + new_packet->get_src_port(),
             new_packet->get_dst_ip() + ":" + new_packet->get_dst_port(),
             new_packet->get_ip_info(),
             new_packet->get_time_added(),
             new_packet->get_packet_spec_info()});

    //Store packet object
    packets.push_back(new_packet);

    //Keep track of index
    switch(new_packet->get_type()) {
    case PacketType::TCP:
        tcp_indices.push_back(num_packets_total++);
        break;
    case PacketType::UDP:
        udp_indices.push_back(num_packets_total++);
        break;
    case PacketType::ICMP:
        icmp_indices.push_back(num_packets_total++);
        break;
    default:
        ip_indices.push_back(num_packets_total++);
        break;
    }

    //Keep track of visibility
    bool type_enabled = types_enabled[new_packet->get_type()];
    setRowHidden(num_packets_total - 1, !type_enabled);
    num_packets_to_display += (int) type_enabled;
}

void PacketTableView::set_ui_closed() {
    ui_closed = true;
}

void PacketTableView::filters_changed(QStandardItem* filter_item) {
    bool filter_enabled = filter_item->checkState() == Qt::Checked;
    QString filter_name = filter_item->text();

    if(filter_name == "TCP") {
        types_enabled[PacketType::TCP] = filter_enabled;
    } else if(filter_name == "UDP") {
        types_enabled[PacketType::UDP] = filter_enabled;
    } else if(filter_name == "ICMP") {
        types_enabled[PacketType::ICMP] = filter_enabled;
    } else {
        qDebug() << "Unknown filter name in PacketTableView::filters_changed(): " << filter_name;
    }

    display_valid_rows();
}

//show tcp, udp, and/or icmp packets
void PacketTableView::display_valid_rows() {
    set_row_visibility_by_index(tcp_indices, types_enabled[PacketType::TCP]);
    set_row_visibility_by_index(udp_indices, types_enabled[PacketType::UDP]);
    set_row_visibility_by_index(icmp_indices, types_enabled[PacketType::ICMP]);

    num_packets_to_display = 0;
    if(types_enabled[PacketType::TCP]) {
        num_packets_to_display += (int) tcp_indices.size();
    }
    if(types_enabled[PacketType::UDP]) {
        num_packets_to_display += (int) udp_indices.size();
    }
    if(types_enabled[PacketType::ICMP]) {
        num_packets_to_display += (int) icmp_indices.size();
    }

    emit num_packets_displayed_changed(num_packets_to_display);
}

void PacketTableView::set_row_visibility_by_index(const std::vector<int>& indices, bool visible) {
    for(auto& index : indices) {
        setRowHidden(index, !visible);
    }
}

//Some ui elements only need to be updated infrequently (to ensure ui responsiveness)
void PacketTableView::update_ui_loop() {
    while(!ui_closed) {
        QThread::msleep(200);
        emit num_packets_displayed_changed(num_packets_to_display);
    }
    qDebug() << "PacketTableView::update_ui_loop() halted";
}

void PacketTableView::clicked_packet(const QModelIndex &index) {
   int row_in_table = index.row();
   qDebug() << "here";
   emit send_clicked_packet(packets[row_in_table]);
}

bool PacketTableView::write_packets_to_file(const std::string& path) {
   packet_update_enabled = false; //Pause updates
   std::ofstream record_file(path);
   if(!record_file) {
        qDebug() << "write_all_packets_to_file() error, could not write file: " << path;
        packet_update_enabled = true; //Unpause updates
        return false;
   }

   for(auto& packet : packets) {
        record_file << packet->get_info().toStdString() << "\n---------------\n";
   }

   record_file << std::endl;
   record_file.close();
   packet_update_enabled = true; //Unpause updates
   return true;
}

int PacketTableView::get_num_packets_displayed() {
   return num_packets_to_display;
}

// A little messy, would prefer to use a thread-safe shared packet vector
// Populate map with packet locations and center on user location (if possible)
void PacketTableView::update_packet_map(CustomMapTab* mapTab) {
   packet_update_enabled = false; //Pause updates
   for(auto& packet : packets) {
        IPCoords coords = packet->get_dst_ip_coords();
        if(coords.known) {
            mapTab->update_map(coords.latitude, coords.longitude, packet->get_dst_geo_info());
        }
   }
   emit all_packets_added_to_map();
   packet_update_enabled = true; //Unpause updates
}
