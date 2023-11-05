/* This file contains definitions for the CustomApplication class
 * (derived from QApplication)
 * Ramsey Nofal, 08/2023
 */


//Local
#include "CustomApplication.h"
#include "StyleWidget.h"


CustomApplication::CustomApplication(int &argc, char **argv)
                                    : QApplication(argc, argv) {
    setWindowIcon(QIcon(QString::fromStdString(ICON_PATH)));
    setStyle("fusion");
}
