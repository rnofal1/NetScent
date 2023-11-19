/* This file contains declarations for the CustomCentralWidget class (derived from QWidget)
 * Ramsey Nofal, 08/2023
 */


#ifndef CUSTOMCENTRALWIDGET_H
#define CUSTOMCENTRALWIDGET_H


//Standard Qt
#include <QWidget>

//Local
#include "StyleWidget.h"


class CustomCentralWidget : public QWidget, StyleWidget {
    Q_OBJECT
public:
    CustomCentralWidget(QWidget *parent = nullptr);
};

#endif // CUSTOMCENTRALWIDGET_H
