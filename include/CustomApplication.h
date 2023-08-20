/* This file contains declarations for the CustomApplication class
 * (derived from QApplication)
 * Ramsey Nofal, 08/2023
 */


#ifndef CUSTOMAPPLICATION_H
#define CUSTOMAPPLICATION_H


//Standard Qt
#include <QApplication>


class CustomApplication : public QApplication {
    Q_OBJECT
public:
    CustomApplication(int &argc, char **argv);
};

#endif // CUSTOMAPPLICATION_H
