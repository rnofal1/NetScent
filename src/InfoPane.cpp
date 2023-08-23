/* This file contains definitions for the InfoPane class (derived from QTextBrowser)
 * Ramsey Nofal, 08/2023
 */


//Local
#include "InfoPane.h"
#include "util.h"

InfoPane::InfoPane(QWidget *parent) : QTextBrowser(parent) {
    add_style("Main", get_stylesheet_from_json("infoPane", "Main"));
    add_style("Alt", get_stylesheet_from_json("infoPane", "Alt"));
}

//ToDo: reduce code duplication with add_style()/set_style() in classes
void InfoPane::add_style(const std::string& style_name, const std::string& style_val) {
    style_map[style_name] = style_val;
}
void InfoPane::set_style(const std::string& style_name) {
    auto style = style_map.find(style_name);

    if(style != style_map.end()) {
        setStyleSheet(QString::fromStdString(style->second));
    }
}
