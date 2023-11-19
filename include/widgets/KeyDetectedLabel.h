/* This file contains declarations for the KeyDetectedLabel class (derived from QLabel)
 * Ramsey Nofal, 11/2023
 */


#ifndef KEYDETECTEDLABEL_H
#define KEYDETECTEDLABEL_H


//Standard Qt
#include <QLabel>

//Local
#include "StyleWidget.h"


class KeyDetectedLabel : public QLabel, public StyleWidget {
    Q_OBJECT
public:
    KeyDetectedLabel(QWidget *parent = nullptr);
    void set_key_detected(const bool key_detected);
};

#endif // KEYDETECTEDLABEL_H
