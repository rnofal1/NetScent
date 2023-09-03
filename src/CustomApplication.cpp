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
    this->setWindowIcon(QIcon(QString::fromStdString(get_json_val(style_json_keys))));
}
