/* This file contains declarations for the StyleWidget class
 * Ramsey Nofal, 08/2023
 */


#ifndef STYLEWIDGET_H
#define STYLEWIDGET_H


/* Standard C/C++ */
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <stdlib.h>
#include <map>

/* Standard Qt */
#include <QString>
#include <QWidget>


/* The purpose of this class is to unify the style-setting interface for all
 * custom Qt-derived classes while minimizing code duplication.
 *
 * ToDo: find a less confusing way to do this (ideally by making a custom QWidget-derived
 * class from which every Qt-derived class will inherit).
 */
class StyleWidget {
public:
    /* Functions */
    StyleWidget(QWidget* widget = nullptr);

    void reset_style();
    void set_style_property(const char* name, const QVariant &value);

protected:
    /* Variables */
    QWidget* this_widget;
};

#endif // STYLEWIDGET_H
