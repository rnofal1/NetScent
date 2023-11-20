/* This file contains declarations for the CustomApplication class
 * (derived from QApplication)
 * Ramsey Nofal, 08/2023
 */


#ifndef CUSTOMAPPLICATION_H
#define CUSTOMAPPLICATION_H


/* Standard Qt */
#include <QApplication>

/* Defines */
#define ICON_PATH "icons/icon.png"
#define APP_STYLE "fusion"


class CustomApplication : public QApplication {
    Q_OBJECT
public:
    /* Functions */
    CustomApplication(int &argc, char **argv);
};

#endif // CUSTOMAPPLICATION_H
