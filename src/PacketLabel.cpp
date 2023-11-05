/* This file contains definitions for the PacketLabel class (derived from QLabel)
 * Ramsey Nofal, 08/2023
 */


//Local
#include "PacketLabel.h"
#include "util.h"
#include "StyleWidget.h"


PacketLabel::PacketLabel(Packet* packet, InfoPane* info_pane_init, QWidget* parent)
                        : QLabel(parent),
                        StyleWidget(this),
                        info_pane(info_pane_init),
                        packet_time(packet->get_time_added()),
                        packet_ip_src(packet->get_src_ip()),
                        packet_ip_dst(packet->get_dst_ip()),
                        packet_num(packet->get_num()) {
    set_style_property("active", false);
}


/* Note: the call to packet->get_packet_geo_info() will trigger a packet capture.
 * Therefore, we cannot call packet->get_packet_geo_info() everytime we capture a
 * packet, therefore it is prudent to not set the geo info within the PacketLabel constructor
 */
void PacketLabel::enterEvent(QEnterEvent *ev) {
    Q_UNUSED(ev);

    set_style_property("active", true);

    if(info_pane != nullptr) {
        info_pane->set_active();

        std::string set_text = "Packet #: ";
        set_text += std::to_string(packet_num);
        set_text += "\nTime Added: ";
        set_text += packet_time;
        set_text += "\n";
        set_text += get_packet_geo_info(packet_ip_src, packet_ip_dst);

        info_pane->setText(QString::fromStdString(set_text));
    }
}

void PacketLabel::leaveEvent(QEvent *ev) {
    Q_UNUSED(ev);
    set_style_property("active", false);
}
