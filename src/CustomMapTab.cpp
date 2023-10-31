/* This file contains definitions for the CustomMapTab class (derived from QWidget)
 * Ramsey Nofal, 08/2023
 */


//Standard Qt
#include "qquickwidget.h"

//Local
#include "CustomMapTab.h"
#include "util.h"

CustomMapTab::CustomMapTab(QWidget *parent) : QWidget(parent), StyleWidget(this) {
    add_style("Main", get_stylesheet_from_json("captureTab", "Main"));
    set_style("Main");
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
    QQuickWidget* quickWidgetMap = this->findChild<QQuickWidget*>("quickWidgetMap");
    if(quickWidgetMap) {
        quickWidgetMap->setSource(QUrl(QStringLiteral("qrc:/QmlMap.qml"))); //Tie this object to the QML
        quickWidgetMap->show();

        auto Obj = quickWidgetMap->rootObject();
        connect(this, SIGNAL(set_map_center(QVariant,QVariant)), Obj, SLOT(set_map_center(QVariant,QVariant)));
        connect(this, SIGNAL(set_map_zoom(QVariant)), Obj, SLOT(set_map_zoom(QVariant)));
        connect(this, SIGNAL(set_location_marker(QVariant,QVariant)), Obj, SLOT(set_location_marker(QVariant,QVariant)));
    } else {
        qInfo() << "Map quickWidget not found\n";
    }
}

// Populate map with packet location and center on user location (if possible)
void CustomMapTab::update_map(const int& point_lat, const float& point_long) {
    // Initialize map if just entering view
    if(!map_active) {
        init_map();
    }

    // Plot location if not already plotted
    auto pair = std::make_pair(point_lat, point_long);
    if(!plotted_locs.count(pair)) {
        plotted_locs.insert(pair);
        emit set_location_marker(point_lat, point_long);
    }
}

// ToDo: delete map (and quickwidget?) when not in view to free up resources
void CustomMapTab::clear_map() {
}

void CustomMapTab::set_map_default_loc() {
    emit set_map_center(US_CENTER_LAT,US_CENTER_LONG);
    emit set_map_zoom(DEFAULT_ZOOM);
}
