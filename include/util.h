/* This file contains declarations for utility functions
 * Ramsey Nofal, 08/2023
 */


#ifndef UTIL_H
#define UTIL_H


//Standard C/C++
#include <fstream>
#include <iostream>
#include <filesystem>

//Standard Qt
#include <QWidget>

//3rd-party
#include <nlohmann/json.hpp>


//Defines
#define API_KEY_FILE "api_key.txt"
#define STYLE_FILE "style/style.json"
#define STATIC_ICON "icons/icon.png"
#define MOVING_ICON "icons/icon_rotate_cube.gif"


//ToDo: modify this to accomodate greater levels of JSON nesting (?) (with a vector of keys(?))
//ToDo: consider removing this and incorporating its functionality into derived UI/GUI classes
void set_stylesheet_from_json(QWidget& widget, const std::string& sec_key, const std::string& sub_sec_key);

#endif // UTIL_H
