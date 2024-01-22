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
    connect(this, SIGNAL(currentChanged(int)), this, SLOT(gen_map()));
}

//ASTAR
/* gen_map() is an optimization: we only want to initialize the map when the map tab is visited
 * (not on startup) to save some resources
 *
 * ToDo: only create qquickwidget on map tab visit (?)
 */
void CustomTabWidget::gen_map() {
    int curr_index = currentIndex();
    if(curr_index == MAP_INDEX || curr_index == ASTAR_INDEX) {
        QString tab_name = curr_index == MAP_INDEX ? MAP_TAB_NAME : ASTAR_TAB_NAME;
        qDebug() << tab_name;
        CustomMapTab *mapTab = this->findChild<CustomMapTab *>(tab_name);
        if(mapTab == nullptr) {
            qDebug() << "Could not find " << tab_name << " in CustomTabWidget::gen_map()";
            return;
        }
        if(!mapTab->is_map_active()) {
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
