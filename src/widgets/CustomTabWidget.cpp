/* This file contains definitions for the CustomTabWidget class (derived from QTabWidget)
 * Ramsey Nofal, 08/2023
 */


/* Local */
#include "CustomTabWidget.h"
#include "CustomMapTab.h"


CustomTabWidget::CustomTabWidget(QWidget *parent) : QTabWidget(parent), StyleWidget(this) {
    connect_signals_slots();
}

void CustomTabWidget::connect_signals_slots() {
    connect(this, SIGNAL(currentChanged(int)), this, SLOT(update_map_tab()));
}

/* update_map_tab() is an optimization: we only want to initialize the map when the map tab is visited
 * (not on startup) to save some resources
 *
 * ToDo: only create qquickwidget on map tab visit (?)
 */
void CustomTabWidget::update_map_tab() {
    if(currentIndex() == MAP_INDEX) {
        CustomMapTab *mapTab = this->findChild<CustomMapTab *>(MAP_TAB_NAME);
        if(mapTab && !mapTab->is_map_active()) {
            mapTab->init_map();
        }
    }
}

//If map is active, tell it to clear any location markers
void CustomTabWidget::clear_map() {
    CustomMapTab *mapTab = this->findChild<CustomMapTab *>(MAP_TAB_NAME);
    if(mapTab && mapTab->is_map_active()) {
        mapTab->clear_map();
    }
}
