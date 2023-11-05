/* This file contains declarations for the CustomTabWidget class (derived from QTabWidget)
 * Ramsey Nofal, 08/2023
 */


#ifndef CUSTOMTABWIDGET_H
#define CUSTOMTABWIDGET_H


//Standard Qt
#include <QTabWidget>

//Local
#include "StyleWidget.h"

//Defines
#define MAP_INDEX 1
#define MAP_TAB_NAME "mapTab"


class CustomTabWidget : public QTabWidget, public StyleWidget {
    Q_OBJECT
public:
    CustomTabWidget(QWidget *parent = nullptr);
    void clear_map();

private slots:
    void update_map_tab();
};


#endif // CUSTOMTABWIDGET_H
