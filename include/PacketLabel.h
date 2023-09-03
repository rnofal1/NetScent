/* This file contains declarations for the PacketLabel class (derived from QLabel)
 * Ramsey Nofal, 08/2023
 */


#ifndef PACKETLABEL_H
#define PACKETLABEL_H


//Standard Qt
#include <QLabel>

//Local
#include "Packet.h"
#include "InfoPane.h"
#include "StyleWidget.h"


/* The CustomLabel class is derived from the QLabel class, with the added functionality
 * of storing packet info
 *
 * CustomLabel is used (in tandem with MainWindow) to display packet info
 */
class PacketLabel : public QLabel, public StyleWidget {
    Q_OBJECT
public:
    PacketLabel(Packet* packet, InfoPane* info_pane_init = nullptr, QWidget* parent = nullptr);

    //PacketLabels handle style slightly differently due to static style_map
    void add_style(const std::string& style_name, const std::string& style_val);
    void set_style(const std::string& style_name);

protected:
    void enterEvent(QEnterEvent *ev) override;
    void leaveEvent(QEvent *ev) override;

private:
    InfoPane* info_pane;
    static std::map<std::string, std::string> style_map;
    std::string packet_time;
    std::string packet_ip_src;
    std::string packet_ip_dst;
    int packet_num;
};

#endif // PACKETLABEL_H
