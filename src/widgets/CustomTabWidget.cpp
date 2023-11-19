/* This file contains definitions for the CustomTabWidget class (derived from QTabWidget)
 * Ramsey Nofal, 08/2023
 */


//Local
#include "CustomTabWidget.h"
#include "CustomMapTab.h"


CustomTabWidget::CustomTabWidget(QWidget *parent) : QTabWidget(parent), StyleWidget(this) {
    connect(this, SIGNAL(currentChanged(int)), this, SLOT(update_map_tab()));
}

// ToDo: only create qquickwidget on map tab visit (?)
void CustomTabWidget::update_map_tab() {
    if(currentIndex() == MAP_INDEX) {
        CustomMapTab *mapTab = this->findChild<CustomMapTab *>(MAP_TAB_NAME);
        if(mapTab && !mapTab->is_map_active()) {
            mapTab->init_map();
        }
    }
}

void CustomTabWidget::clear_map() {
    CustomMapTab *mapTab = this->findChild<CustomMapTab *>(MAP_TAB_NAME);
    if(mapTab && mapTab->is_map_active()) {
        mapTab->clear_map();
    }
}
