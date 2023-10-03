/* This file contains definitions for the CustomCentralWidget class (derived from QWidget)
 * Ramsey Nofal, 08/2023
 */


//Local
#include "CustomCentralWidget.h"
#include "util.h"

CustomCentralWidget::CustomCentralWidget(QWidget *parent) : QWidget(parent), StyleWidget(this) {
    add_style("Main",  get_stylesheet_from_json("centralWidget", "Main"));
    set_style("Main");
}
