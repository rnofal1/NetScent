/* This file contains definitions for the StyleWidget class
 * Ramsey Nofal, 08/2023
 */


//Local
#include "StyleWidget.h"
#include "util.h"

StyleWidget::StyleWidget(QWidget *widget) : this_widget(widget) {}

void StyleWidget::add_style(const std::string& style_name, const std::string& style_val) {
    style_map[style_name] = style_val;
}
void StyleWidget::set_style(const std::string& style_name) {
    auto style = style_map.find(style_name);

    if(style != style_map.end()) {
        this_widget->setStyleSheet(QString::fromStdString(style->second));
    }
}

//ToDo: this is pretty inefficient (and ugly); find a different way to do this
//For now we modify a QWidget arg instead of "this" since not every QWidget inherits from StyleWidget(yet)
void StyleWidget::set_stylesheet_from_json(QWidget& widget, const std::string& sec_key, const std::string& sub_sec_key) {
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

//ToDo: avoid code repetition in this function and set_stylesheet_from_json()
std::string StyleWidget::get_stylesheet_from_json(const std::string& sec_key, const std::string& sub_sec_key) {
    std::ifstream file(STYLE_FILE);
    if(file) {
        nlohmann::json style_json = nlohmann::json::parse(file);
        if(style_json.contains(sec_key) && style_json[sec_key].contains(sub_sec_key)) {
            std::string style_sheet_std;
            for(auto& [key, val] : style_json[sec_key][sub_sec_key].items()) {
                style_sheet_std.append(key).append(" : ").append(val).append("; ");
            }

            return style_sheet_std;
        } else {
            std::cout << "Invalid JSON section + subsection: " << sec_key << " + " << sub_sec_key << "\n";
        }
        file.close();
    } else {
        std::cout << "Style file not found; Dynamic style elements will not work\n";
    }

    return "";
}
