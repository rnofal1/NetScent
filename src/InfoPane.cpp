/* This file contains definitions for the InfoPane class (derived from QTextBrowser)
 * Ramsey Nofal, 08/2023
 */


//Standard Qt
#include <QStyle>

//Local
#include "InfoPane.h"


InfoPane::InfoPane(QWidget *parent) : QTextBrowser(parent), StyleWidget(this) {
    set_inactive();
}

void InfoPane::set_active() {
    set_style_property("active", true);
}
void InfoPane::set_inactive() {
    set_style_property("active", false);
}

void InfoPane::update_text(Packet* packet) {
    std::string set_text = "Packet #: ";
    set_text += std::to_string(packet->get_num() + 1); //+1 for display purposes
    set_text += "\nTime Added: ";
    set_text += packet->get_time_added().toStdString();
    set_text += "\n";
    set_text += packet->get_geo_info();

    setText(QString::fromStdString(set_text));
}
