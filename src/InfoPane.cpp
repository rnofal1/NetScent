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
