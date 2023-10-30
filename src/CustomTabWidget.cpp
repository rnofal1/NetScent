/* This file contains definitions for the CustomTabWidget class (derived from QTabWidget)
 * Ramsey Nofal, 08/2023
 */


//Local
#include "CustomTabWidget.h"
#include "CustomMapTab.h"
#include "util.h"

#define MAP_INDEX 1
#define MAP_TAB_NAME "mapTab"
CustomTabWidget::CustomTabWidget(QWidget *parent) : QTabWidget(parent), StyleWidget(this) {
    add_style("Main", get_stylesheet_from_json("tabWidget", "Main"));
    set_style("Main");

    connect(this, SIGNAL(currentChanged(int)), this, SLOT(update_tab()));
}

// ToDo: only create qquickwidget on map tab visit
void CustomTabWidget::update_tab() {
    if(currentIndex() == MAP_INDEX) {
        CustomMapTab *mapTab = this->findChild<CustomMapTab *>(MAP_TAB_NAME);
        if(mapTab && !mapTab->is_map_active()) {
            mapTab->init_map();
        }
    }
}
