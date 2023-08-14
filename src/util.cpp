/* This file contains definitions for the utility functions declared
 * in util.h
 * Ramsey Nofal, 08/2023
 */


//Local
#include "util.h"


//ToDo: this is pretty inefficient (and ugly); find a different way to do this
void set_stylesheet_from_json(QWidget& widget, const std::string& sec_key, const std::string& sub_sec_key) {
    std::ifstream file(STYLE_FILE);
    if(file) {
        nlohmann::json style_json = nlohmann::json::parse(file);
        if(style_json.contains(sec_key) && style_json[sec_key].contains(sub_sec_key)) {
            std::string style_sheet_std;
            for(auto& [key, val] : style_json[sec_key][sub_sec_key].items()) {
                style_sheet_std.append(key).append(" : ").append(val).append("; ");
            }
            QString style_sheet = QString::fromStdString(style_sheet_std);
            widget.setStyleSheet(style_sheet);
        } else {
            std::cout << "Invalid JSON section + subsection: " << sec_key << " + " << sub_sec_key << "\n";
        }
        file.close();
    } else {
        std::cout << "Style file not found; Dynamic style elements will not work\n";
    }
}
