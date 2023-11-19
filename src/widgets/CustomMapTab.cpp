/* This file contains definitions for the CustomMapTab class (derived from QWidget)
 * Ramsey Nofal, 08/2023
 */


//Standard Qt
#include "qquickwidget.h"

//Local
#include "CustomMapTab.h"
#include "CustomButton.h"


CustomMapTab::CustomMapTab(QWidget *parent) : QWidget(parent), StyleWidget(this) {
    map_active = false;
}

bool CustomMapTab::is_map_active() {
    return map_active;
}

void CustomMapTab::init_map() {
    set_qml();
    set_map_default_loc();
    map_active = true;
}

void CustomMapTab::set_qml() {
    auto quickWidgetMap = this->findChild<QQuickWidget*>("quickWidgetMap");
    Q_CHECK_PTR(quickWidgetMap);

    quickWidgetMap->setSource(QUrl(QStringLiteral("qrc:/QmlMap.qml"))); //Tie this object to the QML
    quickWidgetMap->show();

    auto w_map_obj = quickWidgetMap->rootObject();
    connect(this, SIGNAL(set_map_center(QVariant,QVariant)), w_map_obj, SLOT(set_map_center(QVariant,QVariant)));
    connect(this, SIGNAL(set_map_zoom(QVariant)), w_map_obj, SLOT(set_map_zoom(QVariant)));
    connect(this, SIGNAL(set_location_marker(QVariant,QVariant,QVariant)), w_map_obj, SLOT(set_location_marker(QVariant,QVariant,QVariant)));
    connect(this, SIGNAL(remove_all_location_markers()), w_map_obj, SLOT(remove_all_location_markers()));

    auto mapClearButton = this->findChild<CustomButton*>("mapClearButton");
    Q_CHECK_PTR(mapClearButton);
    connect(mapClearButton, SIGNAL(clicked()), this, SLOT(clear_map()));
}

// Populate map with packet location and center on user location (if possible)
void CustomMapTab::update_map(const int& point_lat, const float& point_long, const std::string& loc_text) {
    // Initialize map if just entering view
    if(!map_active) {
        init_map();
    }

    // Plot location if not already plotted
    auto pair = std::make_pair(point_lat, point_long);
    if(!plotted_locs.count(pair)) {
        plotted_locs.insert(pair);
        emit set_location_marker(point_lat, point_long, QString::fromStdString(loc_text));
    }
}

// ToDo: delete map (and quickwidget?) when not in view to free up resources
void CustomMapTab::clear_map() {
    clear_pins();
}

// Clears location pins from map
void CustomMapTab::clear_pins() {
    plotted_locs.clear();
    emit remove_all_location_markers();
}

void CustomMapTab::set_map_default_loc() {
    emit set_map_center(US_CENTER_LAT,US_CENTER_LONG);
    emit set_map_zoom(DEFAULT_ZOOM);
}
