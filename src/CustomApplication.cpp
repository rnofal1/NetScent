/* This file contains definitions for the CustomApplication class
 * (derived from QApplication)
 * Ramsey Nofal, 08/2023
 */


//Local
#include "CustomApplication.h"
#include "util.h"


CustomApplication::CustomApplication(int &argc, char **argv)
                                    : QApplication(argc, argv) {
    std::ifstream file(STYLE_FILE);
    if(file) {
        nlohmann::json style_json = nlohmann::json::parse(file);
        this->setWindowIcon(QIcon(QString::fromStdString(std::string(style_json["Misc"]["icon-path"]))));
        file.close();
    } else {
        std::cout << "Style file not found; Dynamic style elements will not work\n";
    }
}
