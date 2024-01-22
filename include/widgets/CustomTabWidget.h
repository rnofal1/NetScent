/* This file contains declarations for the CustomTabWidget class (derived from QTabWidget)
 * Ramsey Nofal, 08/2023
 */


#ifndef CUSTOMTABWIDGET_H
#define CUSTOMTABWIDGET_H


/* Standard Qt */
#include <QTabWidget>

/* Local */
#include "StyleWidget.h"

/* Defines */
#define MAP_INDEX 1
#define ASTAR_INDEX 3
#define MAP_TAB_NAME "mapTab"
#define ASTAR_TAB_NAME "astarTab"


class CustomTabWidget : public QTabWidget, public StyleWidget {
    Q_OBJECT
public:
    /* Functions */
    CustomTabWidget(QWidget *parent = nullptr);

    void connect_signals_slots();

    void clear_map();

private slots:
    void gen_map();
};


#endif // CUSTOMTABWIDGET_H
