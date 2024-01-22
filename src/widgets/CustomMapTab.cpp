/* This file contains definitions for the CustomMapTab class (derived from QWidget)
 * Ramsey Nofal, 08/2023
 */


/* Standard Qt */
#include "qquickwidget.h"

/* Local */
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
    auto quickWidgetMap = this->findChild<QQuickWidget*>();
    Q_CHECK_PTR(quickWidgetMap);

    //Tie quickWidgetMap object to the QML
    quickWidgetMap->setSource(QUrl(QStringLiteral("qrc:/QmlMap.qml")));

    quickWidgetMap->show();

    w_map_obj = quickWidgetMap->rootObject();

    //We only need to connect signals/slots once map + qml is initialized
    connect_signals_slots();
}

//ASTAR
void CustomMapTab::connect_signals_slots() {
    //this --> QML
    Q_CHECK_PTR(w_map_obj);
    connect(this, SIGNAL(set_map_center(QVariant,QVariant)), w_map_obj, SLOT(set_map_center(QVariant,QVariant)));
    connect(this, SIGNAL(set_map_zoom(QVariant)), w_map_obj, SLOT(set_map_zoom(QVariant)));
    connect(this, SIGNAL(set_location_marker(QVariant,QVariant,QVariant)), w_map_obj, SLOT(set_location_marker(QVariant,QVariant,QVariant)));
    connect(this, SIGNAL(remove_all_location_markers()), w_map_obj, SLOT(remove_all_location_markers()));
    //ASTAR
    connect(this, SIGNAL(set_shortest_path(QVariant)), w_map_obj, SLOT(show_path(QVariant)));
    //ASTAR
    connect(this, SIGNAL(add_path(QVariant)), w_map_obj, SLOT(add_path(QVariant)));

    //QML --> this
    //ASTAR
    connect(w_map_obj, SIGNAL(hoveredCoordsSignal(double,double,QString)), this, SLOT(find_shortest_path(double,double,QString)));

    //Map clear button --> this
    auto mapClearButton = this->findChild<CustomButton*>("mapClearButton");
    if(mapClearButton == nullptr) {
        mapClearButton = this->findChild<CustomButton*>("astarClearButton");
    }
    Q_CHECK_PTR(mapClearButton);
    connect(mapClearButton, SIGNAL(clicked()), this, SLOT(clear_map()));

    //ASTAR Populate button --> this
    auto populateButton = this->findChild<CustomButton*>("populateButton");
    if(populateButton != nullptr) {
        connect(populateButton, SIGNAL(clicked()), this, SLOT(astar_populate()));
    }
}

// Populate map with packet location and center on user location (if possible)
void CustomMapTab::update_map(const float& point_lat, const float& point_long, const QString& loc_text) {
    // Initialize map if just entering view
    if(!map_active) {
        init_map();
    }

    // Plot location if not already plotted
    auto pair = std::make_pair(point_lat, point_long);
    if(!plotted_locs.count(pair)) {
        plotted_locs.insert(pair);
        qDebug() << point_lat << " " << point_long;
        emit set_location_marker(point_lat, point_long, loc_text);
    }
}

//ASTAR
void CustomMapTab::astar_populate() {
    astar_worker = new AStar();
    connect_astar_graph_locs();
    astar_worker->print_graph();

    for(const auto& node : astar_worker->graph) {
        emit set_location_marker(node->lat, node->lon, node->name);
    }
}

//ASTAR
void CustomMapTab::connect_astar_graph_locs() {
    for(const auto& [start_node, end_node] : astar_worker->edges) {
        add_new_path(start_node->lat, start_node->lon, end_node->lat, end_node->lon);
    }
}

//ASTAR
void CustomMapTab::add_new_path(double orig_lat, double orig_lon, double dest_lat, double dest_lon) {
    QVariantList path_line;

    QGeoCoordinate *orig_coord = new QGeoCoordinate(orig_lat, orig_lon, 0);
    QGeoCoordinate *dest_coord = new QGeoCoordinate(dest_lat, dest_lon, 0);

    if (orig_coord->isValid()) {
        path_line.append(QVariant::fromValue(*orig_coord));
    }
    if (dest_coord->isValid()) {
        path_line.append(QVariant::fromValue(*dest_coord));
    }

    emit add_path(path_line);
}

//ASTAR
void CustomMapTab::find_shortest_path(double dest_lat, double dest_lon, QString dest_name) {
    qDebug() << dest_lat << " " << dest_lon << dest_name;

    std::vector<std::vector<double>> shortest_path_vec = astar_worker->find_shortest_path("My Location", dest_name);

    shortest_path.clear();
    for(auto& coord : shortest_path_vec) {
        QGeoCoordinate *coord_obj = new QGeoCoordinate(coord[0], coord[1], 0);
        if (coord_obj->isValid()) {
            shortest_path.append(QVariant::fromValue(*coord_obj));
        }
    }

    emit set_shortest_path(shortest_path);
}
//ASTAR
//NEEDS: list of nodes, weights (distances) between nodes, start node, end node
//RETURNS: in-order list of nodes to visit, represented by lat/long coordinates
void CustomMapTab::astar_build_shortest_path() {

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
