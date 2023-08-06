/* This file contains definitions for the CustomLabel class (derived from QLabel)
 * Ramsey Nofal, 08/2023
 */


//Local
#include "customlabel.h"


CustomLabel::CustomLabel(Packet* packet, QTextBrowser* info_pane_init, QWidget* parent) :
                         QLabel(parent),
                         info_pane(info_pane_init),
                         packet(packet) {}

//ToDo: store style specifications somewhere
void CustomLabel::enterEvent(QEnterEvent *ev) {
    Q_UNUSED(ev);
    setStyleSheet("QLabel { background-color : grey; }");

    if(info_pane != nullptr) {
        QTextStream(stdout) << "Update Infopane\n";
        info_pane->setStyleSheet("QTextBrowser { background-color : lightgrey; }");

        std::string set_text = "Packet #: " + std::to_string(packet->get_num())
                               +"\nTime Added: " + packet->get_time_added()
                               +"\n" + packet->get_packet_geo_info();

        info_pane->setText(QString::fromStdString(set_text));
    }
}

void CustomLabel::leaveEvent(QEvent *ev) {
    Q_UNUSED(ev);
    setStyleSheet("QLabel { background-color : white; }");
}
