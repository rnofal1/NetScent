/* This file contains definitions for the CustomTabWidget class (derived from QTabWidget)
 * Ramsey Nofal, 08/2023
 */


//Local
#include "CustomTabWidget.h"
#include "util.h"

CustomTabWidget::CustomTabWidget(QWidget *parent) : QTabWidget(parent), StyleWidget(this) {
    add_style("Main", get_stylesheet_from_json("tabWidget", "Main"));
    set_style("Main");
}
