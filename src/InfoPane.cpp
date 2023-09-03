/* This file contains definitions for the InfoPane class (derived from QTextBrowser)
 * Ramsey Nofal, 08/2023
 */


//Local
#include "InfoPane.h"
#include "util.h"

InfoPane::InfoPane(QWidget *parent) : QTextBrowser(parent), StyleWidget(this) {
    add_style("Main", get_stylesheet_from_json("infoPane", "Main"));
    add_style("Alt", get_stylesheet_from_json("infoPane", "Alt"));
}
