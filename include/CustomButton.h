/* This file contains declarations for the CustomButton class (derived from QPushButton)
 * Ramsey Nofal, 08/2023
 */


#ifndef CUSTOMBUTTON_H
#define CUSTOMBUTTON_H

//Standard Qt
#include <QPushButton>

//Local
#include "StyleWidget.h"


class CustomButton : public QPushButton, public StyleWidget {
    Q_OBJECT
public:
    CustomButton(QWidget *parent = nullptr);

    void enable();
    void disable();

protected:
    void enterEvent(QEnterEvent *ev) override;
    void leaveEvent(QEvent *ev) override;
};

#endif // CUSTOMBUTTON_H
