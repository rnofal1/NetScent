/* This file contains declarations for the CustomTabWidget class (derived from QTabWidget)
 * Ramsey Nofal, 08/2023
 */


#ifndef CUSTOMTABWIDGET_H
#define CUSTOMTABWIDGET_H


//Standard Qt
#include <QTabWidget>

//Local
#include "StyleWidget.h"


class CustomTabWidget : public QTabWidget, public StyleWidget {
    Q_OBJECT
public:
    CustomTabWidget(QWidget *parent = nullptr);

private slots:
    void update_tab();
};


#endif // CUSTOMTABWIDGET_H
