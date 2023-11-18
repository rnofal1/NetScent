/* This file contains declarations for the PacketTableView class (derived from QTableView)
 * Ramsey Nofal, 08/2023
 */


#ifndef PACKETSCROLLAREA_H
#define PACKETSCROLLAREA_H


//Standard Qt
//#include <QScrollArea>
//#include <QScrollBar>
#include <QThread>
#include <QFuture>
#include <QTableView>
#include <QStandardItemModel>

//Local
#include "StyleWidget.h"
#include "InfoPane.h"
#include "SharedQueue.h"
#include "CustomMapTab.h"


class PacketTableView : public QTableView, public StyleWidget {
    Q_OBJECT
public:
    PacketTableView(QWidget *parent = nullptr);
    void add_row(const std::vector<QString>& data);

    void set_packet_queue(SharedQueue<Packet*> *packet_queue);
    void start_queue_poll();

    bool write_packets_to_file(const std::string& path);

    int get_num_packets_displayed();

    // A little messy, would prefer to use a thread-safe shared packet vector
    // Populate map with packet locations and center on user location (if possible)
    void update_packet_map(CustomMapTab* mapTab);

private slots:
    void clear_view();
    void set_ui_closed();
    void filters_changed(QStandardItem* filter_item);
    void clicked_packet(const QModelIndex &index); //To relay clicked() signal + packet info without modifying clicked()


signals:
    void num_packets_displayed_changed(int num_packets);
    void send_clicked_packet(Packet* packet); //To relay clicked() signal + packet info without modifying clicked()
    void all_packets_added_to_map();

private:
    bool packet_update_enabled;

    bool ui_closed;

    std::map<PacketType, bool> types_enabled = {
        {PacketType::TCP, true},
        {PacketType::UDP, true},
        {PacketType::ICMP, true},
        {PacketType::IP, true}
    };

    std::vector<int> tcp_indices;
    std::vector<int> udp_indices;
    std::vector<int> icmp_indices;
    std::vector<int> ip_indices; //not used (yet)

    int num_packets_total;
    int num_packets_to_display;

    QStandardItemModel* model; //ToDo: consider using a QAbstractTableModel

    InfoPane* info_pane;

    QFuture<void> poll_queue_thread;

    QFuture<void> ui_update_thread;

    std::vector<Packet*> packets;

    SharedQueue<Packet*> *packet_queue;

    std::vector<QString> model_col_headers = {
        "Type",
        "Src IP",
        "Dst IP",
        "IP Info",
        "Time Added",
        "Misc Info"
    };

    void config_model();
    void config_mode_options();

    void poll_queue();

    void add_packet(Packet* new_packet);

    void display_valid_rows(); //Show tcp, udp, and/or icmp packets

    void set_row_visibility_by_index(const std::vector<int>& indices, bool visible);

    void update_ui_loop();
};

#endif // PACKETSCROLLAREA_H
