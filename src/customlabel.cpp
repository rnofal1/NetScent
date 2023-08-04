//Local
#include "customlabel.h"


CustomLabel::CustomLabel(Packet* packet, QTextBrowser* infoPaneInit, QWidget* parent) :
            QLabel(parent),
            infoPane(infoPaneInit),
            packet(packet) {}

void CustomLabel::enterEvent(QEnterEvent *ev) {
    Q_UNUSED(ev); //Suppress Qt Creator unused parameter warning
    setStyleSheet("QLabel { background-color : grey; }");

    if(infoPane != nullptr) {
        QTextStream(stdout) << "Update Infopane\n";
        infoPane->setStyleSheet("QTextBrowser { background-color : lightgrey; }");

        std::string set_text = "Packet #: " + std::to_string(packet->get_num())
                               +"\nTime Added: " + packet->get_time_added()
                               +"\n" + packet->get_packet_geo_info();

        infoPane->setText(QString::fromStdString(set_text));
    }
}

void CustomLabel::leaveEvent(QEvent *ev) {
    Q_UNUSED(ev); //Suppress Qt Creator unused parameter warning
    setStyleSheet("QLabel { background-color : white; }");

//    if(infoPane != nullptr) {
//        infoPane->setStyleSheet("QTextBrowser { background-color : white; }");
//        infoPane->clear();
//    }
}
