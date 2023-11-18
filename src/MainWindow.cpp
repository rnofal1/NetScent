/* This file contains definitions for the MainWindow class (derived from QMainWindow)
 * Ramsey Nofal, 08/2023
 */


//Standard Qt
#include <QtConcurrent>

//Local
#include "MainWindow.h"
#include "PacketLabel.h"
#include "util.h"
#include "LoadingLabel.h"


MainWindow::MainWindow(SharedQueue<Packet*> *packet_queue, QWidget *parent)
                        : QMainWindow(parent),
                        StyleWidget(this),
                        run_capture(false),
                        closed(false),
                        clear_packets(false),
                        packet_queue(packet_queue),
                        num_displayed_packets(0) {
    ui = new Ui::MainWindow();
    ui->setupUi(this);

    set_widgets_style();

    connect_signals_slots();

    init_packet_view();
}

MainWindow::~MainWindow() {
    emit ui_closed();
    delete_packets();
    delete ui;
}

Ui::MainWindow* MainWindow::get_ui_pointer() {
    return this->ui;
}

QString MainWindow::get_stylesheet() {
    QFile styleFile(STYLE_FILE);
    styleFile.open(QFile::ReadOnly);
    QString style(styleFile.readAll());
    styleFile.close();
    return style;
}

//ToDo: all children should inherit the MainWindow stylesheet, but it seems like they don't (this shouldn't be necessary)
void MainWindow::set_child_stylesheets() {
    QString style = get_stylesheet();
    setStyleSheet(style);
    QList<QWidget*> children = findChildren<QWidget*>();
    for(auto& child : children) {
        auto child_style = dynamic_cast<StyleWidget*>(child);
        if(child_style) {
            child_style->reset_style();
        } else {
            child->setStyleSheet(style);
        }
    }

    this->ensurePolished();
}

void MainWindow::set_widgets_style() {
    //ApiLinkLabel Style
    ui->apiLinkLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
    ui->apiLinkLabel->setToolTip("Follow this link, create an account, and paste your API key at"
                                 " the right to unlock IP geolocation");

    //Tabs Style
    ui->tabWidget->setCurrentIndex(0);

    //Disable certain buttons
    ui->saveButton->disable();

    update_api_key_status();

    set_child_stylesheets();
}

void MainWindow::connect_signals_slots() {
    //Buttons
    connect(ui->startButton, SIGNAL(clicked()), this, SLOT(start_button_clicked()));
    connect(ui->stopButton, SIGNAL(clicked()), this, SLOT(stop_button_clicked()));
    connect(ui->setApiKeyButton, SIGNAL(clicked()), this, SLOT(set_api_button_clicked()));
    connect(ui->packetClearButton, SIGNAL(clicked()), this, SLOT(remove_existing_packets()));
    connect(ui->packetClearButton, SIGNAL(clicked()), ui->packetTableView, SLOT(clear_view()));
    connect(ui->saveButton, SIGNAL(clicked()), this, SLOT(save_to_file()));
    connect(ui->mapRefreshButton, SIGNAL(clicked()), this, SLOT(refresh_button_clicked()));

    //Packet filters
    //connect(&ui->filterBox->model, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(refresh_packet_window()));
    connect(&ui->filterBox->model, SIGNAL(itemChanged(QStandardItem*)), ui->packetTableView, SLOT(filters_changed(QStandardItem*)));



    //Shared queue
    connect(this, SIGNAL(new_packet_ready()), this, SLOT(display_next_packet()));

    //Packet Table View
    connect(this, SIGNAL(ui_closed()), ui->packetTableView, SLOT(set_ui_closed()));
    connect(ui->packetTableView, SIGNAL(send_clicked_packet(Packet*)), this, SLOT(update_info_pane(Packet*)));
    connect(ui->packetTableView, SIGNAL(num_packets_displayed_changed(int)), ui->numPacketsDisplay, SLOT(set_num_slot(int)));
    connect(ui->packetTableView, SIGNAL(all_packets_added_to_map()), this, SLOT(map_update_complete()));
}

void MainWindow::init_packet_view() {
    ui->packetTableView->set_packet_queue(packet_queue);
    ui->packetTableView->start_queue_poll();
}

// ToDo: consider condensing these
void MainWindow::set_packet_loading_active() {
    auto packetLoadingLabel = ui->packetLoadingLabel;
    packetLoadingLabel->set_status_active();
}
void MainWindow::set_packet_loading_inactive() {
    auto packetLoadingLabel = ui->packetLoadingLabel;
    packetLoadingLabel->set_status_inactive();
}
void MainWindow::set_map_loading_active() {
    auto mapLoadingLabel = ui->mapLoadingLabel;
    mapLoadingLabel->set_status_active();
}
void MainWindow::set_map_loading_inactive() {
    auto mapLoadingLabel = ui->mapLoadingLabel;
    mapLoadingLabel->set_status_inactive();
}

 //Continuously add elements from shared Packet queue
void MainWindow::poll_queue() {
    while(!closed) {
        if(!packet_queue->is_empty()) {
            emit new_packet_ready();
        }
        QThread::msleep(200); //Prevents GUI from being inundated with too many packets
    }
    qDebug() << "Polling halted";
}

//void MainWindow::display_next_packet() {
//    Packet* next = packet_queue->pop();
//    if(next) {
//        packets.push_back(next);

//        bool tcp_enabled = ui->filterBox->tcp_filter_enabled();
//        bool udp_enabled = ui->filterBox->udp_filter_enabled();
//        bool icmp_enabled = ui->filterBox->icmp_filter_enabled();
//        if(tcp_enabled && dynamic_cast<TCPPacket*>(next)) {
//            display_packet(next);
//        } else if(udp_enabled && dynamic_cast<UDPPacket*>(next)) {
//            display_packet(next);
//        } else if(icmp_enabled && dynamic_cast<ICMPPacket*>(next)) {
//            display_packet(next);
//        }
//    }
//}


//void MainWindow::display_packet(Packet* packet) {
//    num_displayed_packets++;
//    ui->numPacketsDisplay->set_num(num_displayed_packets);

//    ui->packetTableView->add_row({packet->get_type_name(),
//                                  packet->get_src_ip() + ":" + packet->get_src_port(),
//                                  packet->get_dst_ip() + ":" + packet->get_dst_port(),
//                                  packet->get_ip_info(),
//                                  packet->get_time_added(),
//                                  packet->get_packet_spec_info()});

//    ui->packetTableView->add_packet_index(packet->get_num());
//}

void MainWindow::closeEvent(QCloseEvent *ev) {
    Q_UNUSED(ev);
    closed = true;
}

void MainWindow::start_button_clicked() {
    ui->saveButton->disable();
    set_packet_loading_active();
    run_capture = true;
}
void MainWindow::stop_button_clicked() {
    set_packet_loading_inactive();
    run_capture = false;

    if(ui->packetTableView->get_num_packets_displayed() > 0) {
        ui->saveButton->enable();
    }

//    if(!packet_queue->is_empty()) {
//        Packet* next_packet = packet_queue->pop();
//        packets.push_back(next_packet);
//        display_packet(next_packet);
//    }
}

void MainWindow::clear_packet_display() {
    num_displayed_packets = 0;

    //ui->packetTableView->clear_view();

    ui->infoPane->clear();
    ui->infoPane->set_inactive();
}
void MainWindow::remove_existing_packets() {
    stop_button_clicked();
    clear_packets = true;
    clear_packet_display();
    delete_packets();
    ui->tabWidget->clear_map();
    ui->numPacketsDisplay->set_num(0);
    ui->saveButton->disable();
}

//ToDo: URGENT Handle api key(s) in a more sensible way
void MainWindow::set_api_button_clicked() {
    std::string input_key = ui->apiKeyText->text().toStdString();

    if(input_key != dummy_api_key) {
        std::fstream api_key_file(API_KEY_FILE, std::fstream::out | std::fstream::trunc);
        if(api_key_file) {
            api_key_file << input_key << std::endl;
            api_key_file.close();
        } else {
            qDebug() << "API Key file write error";
        }
    }

    update_api_key_status();
}

//ToDo: URGENT Handle api key(s) in a more sensible way
//ToDo: just use the function in util
void MainWindow::update_api_key_status() {
    std::fstream api_key_file(API_KEY_FILE, std::fstream::in | std::fstream::app);
    std::string key;

    if(api_key_file) {
        std::getline(api_key_file, key);
        api_key_file.close();
    } else {
        qDebug() << "API Key file read error";
    }

    if(key.length() > 0) {
        dummy_api_key = std::string(key.length(), '*');
        ui->apiKeyText->setText(QString::fromStdString(dummy_api_key));
        ui->keyDetectedLabel->set_key_detected(true);
    } else {
        ui->keyDetectedLabel->set_key_detected(false);
    }
}

void MainWindow::refresh_packet_window() {
    //clear_packet_display();
    //add_valid_packets();
}

//ToDo: give user more control over save file
void MainWindow::save_to_file() {
    std::string initial_path = get_dir_path_from_user();
    if(initial_path == "") {
        return;
    }
    std::string record_file_name = create_record_file_name();
    std::string full_path = initial_path + "/" + record_file_name;

    bool write_success = ui->packetTableView->write_packets_to_file(full_path);
    if(write_success) {
        message_popup("Packet record saved as: " + full_path);
    } else {
        message_popup("ERROR: record not saved");
    }
}


//ToDo: this is bad; make this more efficient; loop through all available filter options
//ToDo: store indices + numbers of each type of packet as the packets come in (?)
//void MainWindow::add_valid_packets() {
//    bool tcp_enabled = ui->filterBox->tcp_filter_enabled();
//    bool udp_enabled = ui->filterBox->udp_filter_enabled();
//    bool icmp_enabled = ui->filterBox->icmp_filter_enabled();

//    for(auto& packet : packets) {
//        if(tcp_enabled && dynamic_cast<TCPPacket*>(packet)) {
//            display_packet(packet);
//        } else if(udp_enabled && dynamic_cast<UDPPacket*>(packet)) {
//            display_packet(packet);
//        } else if(icmp_enabled && dynamic_cast<ICMPPacket*>(packet)) {
//            display_packet(packet);
//        }
//    }
//}

void MainWindow::delete_packets() {
    std::for_each(packets.begin(), packets.end(), delete_ptr());
    packets.clear();
}

void MainWindow::message_popup(const std::string& msg) {
    QMessageBox msgBox;

    msgBox.setText(QString::fromStdString(msg));
    msgBox.exec();
}

void MainWindow::refresh_button_clicked() {
    stop_button_clicked();
    ui->packetClearButton->disable();
    ui->mapClearButton->disable();
    ui->startButton->disable();
    set_map_loading_active();
    QFuture<void> map_update_thread = QtConcurrent::run([this] {update_map();});
}

void MainWindow::update_map() {
    ui->packetTableView->update_packet_map(ui->mapTab);
}

void MainWindow::map_update_complete() {
    set_map_loading_inactive();
    ui->packetClearButton->enable();
    ui->mapClearButton->enable();
    ui->startButton->enable();
}

std::string MainWindow::get_dir_path_from_user() {
    std::string folder_path = get_cwd() + "/Records";

    try {
        std::filesystem::create_directory(folder_path);
    } catch(...){
        auto err_msg = "ERROR: Could not create directory at path: " + folder_path;
        qDebug() << err_msg;
        message_popup(err_msg);
        return "";
    }

    QString path = QFileDialog::getExistingDirectory(this,
                                                         tr("Choose Save Folder"),
                                                         QString::fromStdString(folder_path),
                                                         QFileDialog::ShowDirsOnly
                                                         );
    return path.toStdString();
}

std::string MainWindow::create_record_file_name() {
    std::time_t time_saved = std::time(nullptr);

    std::string record_path_preset ="PacketRecord_";
    record_path_preset += std::asctime(std::localtime(&time_saved));
    record_path_preset += ".txt";
    record_path_preset.erase(std::remove(record_path_preset.begin(), record_path_preset.end(), '\n'), record_path_preset.end());
    std::replace(record_path_preset.begin(), record_path_preset.end(), ' ', '_');
    std::replace(record_path_preset.begin(), record_path_preset.end(), ':', '#');

    return record_path_preset;
}

//Return true on success, false otherwise
//bool MainWindow::write_all_packets_to_file(const std::string& path) {
//    std::ofstream record_file(path);
//    if(!record_file) {
//        qDebug() << "write_all_packets_to_file() error, could not write file: " << path;
//        return false;
//    }

//    for(auto& packet : packets) {
//        record_file << packet->get_info().toStdString() << "\n---------------\n";
//    }

//    record_file << std::endl;
//    record_file.close();
//    return true;
//}

void MainWindow::update_info_pane(Packet* packet) {
    ui->infoPane->set_active();
    ui->infoPane->update_text(packet);
}
