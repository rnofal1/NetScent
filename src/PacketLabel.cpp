/* This file contains definitions for the PacketLabel class (derived from QLabel)
 * Ramsey Nofal, 08/2023
 */


//Local
#include "PacketLabel.h"
#include "util.h"
#include "StyleWidget.h"

//Static variable definitions
/* Note: making this map static greatly improves performance since we usually
 * have many PacketLabels (which should all look identical and thus have the same style parameters).
 *
 * ToDo: despite the performance gain, there are other/better ways of doing this
 */
std::map<std::string, std::string> PacketLabel::style_map = {{"Main",
                                                            StyleWidget::get_stylesheet_from_json("packetLabel", "Main")},
                                                            {"Alt",
                                                            StyleWidget::get_stylesheet_from_json("packetLabel", "Alt")}};

PacketLabel::PacketLabel(Packet* packet, InfoPane* info_pane_init, QWidget* parent)
                        : QLabel(parent),
                        StyleWidget(this),
                        info_pane(info_pane_init),
                        packet_time(packet->get_time_added()),
                        packet_ip_src(packet->get_src_ip()),
                        packet_ip_dst(packet->get_dst_ip()),
                        packet_num(packet->get_num()) {}

//PacketLabels handle style slightly differently due to static style_map
void PacketLabel::add_style(const std::string& style_name, const std::string& style_val) {
    style_map[style_name] = style_val;
}
void PacketLabel::set_style(const std::string& style_name) {
    auto style = style_map.find(style_name);

    if(style != style_map.end()) {
        setStyleSheet(QString::fromStdString(style->second));
    }
}

/* Note: the call to packet->get_packet_geo_info() will trigger a packet capture.
 * Therefore, we cannot call packet->get_packet_geo_info() everytime we capture a
 * packet, therefore it is prudent to not set the geo info within the PacketLabel constructor
 */
void PacketLabel::enterEvent(QEnterEvent *ev) {
    Q_UNUSED(ev);

    set_style("Alt");

    if(info_pane != nullptr) {
        info_pane->set_style("Alt");

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
    set_style("Main");
}
