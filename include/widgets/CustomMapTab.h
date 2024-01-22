/* This file contains declarations for the CustomMapTab class (derived from QWidget)
 * Ramsey Nofal, 10/2023
 */


#ifndef CUSTOMMAPTAB_H
#define CUSTOMMAPTAB_H


/* Standard C/C++ */
#include <set>
#include <winsock2.h> // This needs to go before QtQuick
#include <stdlib.h>

/* Standard Qt */
#include <QWidget>
#include <QObject>
#include <QVariant>
#include <QtQuick>
#include <QGeoCoordinate>

/* Local */
#include "StyleWidget.h"
#include "AStar.h"

/* Defines */
#define US_CENTER_LAT 39.8283
#define US_CENTER_LONG -98.5795
#define DEFAULT_ZOOM 3


class CustomMapTab : public QWidget, public StyleWidget {
    Q_OBJECT
public:
    /* Functions */
    CustomMapTab(QWidget *parent = nullptr);

    bool is_map_active();

    void set_qml();

    void connect_signals_slots();

    void init_map();

    void clear_pins();

    void update_map(const float& point_lat, const float& point_long, const QString& loc_text);

private:
    /* Variables */
    QQuickItem *w_map_obj;

    //ASTAR ---- START
    std::vector<std::vector<float>> random_coords = {
        {41.8781, -87.6298},
        {40.730610, -73.935242},
        {34.052235, -118.243683},
        {29.716131, -95.388718},
        {33.448376, -112.074036},
        {39.952583, -75.165222},
        {29.424349, -98.491142},
        {32.715736, -117.161087},
        {32.779167, -96.808891},
        {37.335480, -121.893028},
        {25.782721, -80.140556},
        {41.257160, -95.995102},
        {39.742043, -104.991531},
        {36.174465, -86.767960},
        {44.967243, -103.771556}
    };
    std::vector<QString> random_coords_text = {
        {"Chicago"},
        {"New York"},
        {"Los Angeles"},
        {"Houston"},
        {"Phoenix"},
        {"Philadelphia"},
        {"San Antonio"},
        {"San Diego"},
        {"Dallas"},
        {"San Jose"},
        {"Miami"},
        {"Omaha"},
        {"Denver"},
        {"Nashville"},
        {"My Location"}
    };
    QVariantList shortest_path;
    //ASTAR ---- END

    std::set<std::pair<float, float>> plotted_locs;

    bool map_active;

    //ASTAR
    AStar* astar_worker;

    /* Functions */
    void set_map_default_loc();

    //ASTAR
    void astar_build_shortest_path();
    //ASTAR
    void add_new_path(double orig_lat, double orig_lon, double dest_lat, double dest_lon); //Add path permanently to the map
    //ASTAR
    void connect_astar_graph_locs();//Connect the graph nodes on the map with edges

signals:
    void set_map_center(QVariant, QVariant);
    void set_map_zoom(QVariant);
    void set_location_marker(QVariant, QVariant, QVariant);
    void remove_all_location_markers();
    //ASTAR
    void set_shortest_path(QVariant); //Send shortest path to QML
    //ASTAR
    void add_path(QVariant); //Adds a path permanently to the map

public slots:
    void clear_map();
    //ASTAR
    void astar_populate();//Add random points to map
    //ASTAR
    void find_shortest_path(double dest_lat, double dest_long, QString dest_name);
};

#endif // CUSTOMMAPTAB_H
