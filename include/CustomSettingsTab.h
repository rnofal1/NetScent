/* This file contains declarations for the CustomSettingsTab class (derived from QWidget)
 * Ramsey Nofal, 11/2023
 */


#ifndef CUSTOMSETTINGSTAB_H
#define CUSTOMSETTINGSTAB_H


//Standard Qt
#include <QWidget>

//Local
#include "StyleWidget.h"


class CustomSettingsTab : public QWidget, public StyleWidget {
    Q_OBJECT
public:
    CustomSettingsTab(QWidget *parent = nullptr);
};


#endif // CUSTOMSETTINGSTAB_H
