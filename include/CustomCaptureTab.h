/* This file contains declarations for the CustomCaptureTab class (derived from QWidget)
 * Ramsey Nofal, 08/2023
 */


#ifndef CUSTOMCAPTURETAB_H
#define CUSTOMCAPTURETAB_H


//Standard Qt
#include <QWidget>

//Local
#include "StyleWidget.h"

class CustomCaptureTab : public QWidget, public StyleWidget {
    Q_OBJECT
public:
    CustomCaptureTab(QWidget *parent = nullptr);
};


#endif // CUSTOMCAPTURETAB_H
