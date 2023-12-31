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

/* Local */
#include "StyleWidget.h"

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

    void update_map(const int& point_lat, const float& point_long, const QString& loc_text);

private:
    /* Variables */
    QQuickItem *w_map_obj;

    std::set<std::pair<float, float>> plotted_locs;

    bool map_active;

    /* Functions */
    void set_map_default_loc();

signals:
    void set_map_center(QVariant, QVariant);
    void set_map_zoom(QVariant);
    void set_location_marker(QVariant, QVariant, QVariant);
    void remove_all_location_markers();

public slots:
    void clear_map();
};

#endif // CUSTOMMAPTAB_H
