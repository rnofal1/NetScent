/* This file contains definitions for the PacketLabel class (derived from QLabel)
 * Ramsey Nofal, 08/2023
 */


//Local
#include "PacketLabel.h"
#include "StyleWidget.h"


PacketLabel::PacketLabel(Packet* packet, InfoPane* info_pane_init, QWidget* parent)
                        : QLabel(parent),
                        StyleWidget(this),
                        info_pane(info_pane_init),
                        packet(packet),
                        packet_time(packet->get_time_added()),
                        packet_ip_src(packet->get_src_ip()),
                        packet_ip_dst(packet->get_dst_ip()),
                        packet_num(packet->get_num() + 1) { //Add 1 for display purposes
    set_style_property("active", false);
    geo_info = ""; //ToDo: might change this
}


/* Note: the call to packet->get_packet_geo_info() will trigger a packet capture.
 * Therefore, we cannot call packet->get_packet_geo_info() everytime we capture a
 * packet, therefore it is prudent to not set the geo info within the PacketLabel constructor
 */
void PacketLabel::enterEvent(QEnterEvent *ev) {
    Q_UNUSED(ev);

    set_style_property("active", true);

    if(info_pane == nullptr) {
        qDebug() << "No info pane pointer in PacketLabel";
        return;
    }
    if(packet == nullptr) {
        qDebug() << "Packet in PacketLabel is nullptr";
        return;
    }

    if(geo_info.length() == 0) {
        geo_info = packet->get_geo_info();
    }

    info_pane->set_active();

    std::string set_text = "Packet #: ";
    set_text += std::to_string(packet_num);
    set_text += "\nTime Added: ";
    set_text += packet_time;
    set_text += "\n";
    set_text += geo_info;

    info_pane->setText(QString::fromStdString(set_text));
}

void PacketLabel::leaveEvent(QEvent *ev) {
    Q_UNUSED(ev);
    set_style_property("active", false);
}
