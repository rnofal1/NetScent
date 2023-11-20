/* This file contains definitions for the InfoPane class (derived from QTextBrowser)
 * Ramsey Nofal, 08/2023
 */


/* Standard Qt */
#include <QStyle>

/* Local */
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
    QString set_text = "Packet #: ";
    set_text += QString::number(packet->get_num() + 1); //+1 for display purposes
    set_text += "\nTime Added: ";
    set_text += packet->get_time_added();
    set_text += "\n";
    set_text += packet->get_geo_info();

    setText(set_text);
}
