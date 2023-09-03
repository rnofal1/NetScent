/* This file contains definitions for the StyleWidget class
 * Ramsey Nofal, 08/2023
 */


//Local
#include "StyleWidget.h"


StyleWidget::StyleWidget(QWidget *widget) : this_widget(widget) {}

void StyleWidget::add_style(const std::string& style_name, const std::string& style_val) {
    style_map[style_name] = style_val;
}
void StyleWidget::set_style(const std::string& style_name) {
    auto style = style_map.find(style_name);

    if(style != style_map.end()) {
        this_widget->setStyleSheet(QString::fromStdString(style->second));
    }
}
