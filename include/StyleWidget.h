/* This file contains declarations for the StyleWidget class
 * Ramsey Nofal, 08/2023
 */


#ifndef STYLEWIDGET_H
#define STYLEWIDGET_H


//Standard C++
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <stdlib.h>
#include <map>

//Standard Qt
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
    StyleWidget(QWidget* widget = nullptr);

    void add_style(const std::string& style_name, const std::string& style_val);
    void set_style(const std::string& style_name);

protected:
    QWidget* this_widget;
    std::map<std::string, std::string> style_map;
};

#endif // STYLEWIDGET_H