/* This file contains definitions for the CustomApplication class
 * (derived from QApplication)
 * Ramsey Nofal, 08/2023
 */


//Local
#include "CustomApplication.h"
#include "util.h"


CustomApplication::CustomApplication(int &argc, char **argv)
                                    : QApplication(argc, argv) {
    std::vector<std::string> style_json_keys = {"Misc", "icon-path"};
    std::string icon_path = get_json_val(style_json_keys);
    add_style("icon_path", icon_path);
    set_style("icon_path");
}

void CustomApplication::add_style(const std::string& style_name, const std::string& style_val) {
    style_map[style_name] = style_val;
}
void CustomApplication::set_style(const std::string& style_name) {
    auto style = style_map.find(style_name);

    if(style != style_map.end()) {
        this->setWindowIcon(QIcon(QString::fromStdString(style->second)));
    }

    setStyle("fusion"); //NOTE: different from set_style (necessary for tab style in Windows)
}
