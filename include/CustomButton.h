/* This file contains declarations for the CustomButton class (derived from QPushButton)
 * Ramsey Nofal, 08/2023
 */


#ifndef CUSTOMBUTTON_H
#define CUSTOMBUTTON_H

//Standard Qt
#include <QPushButton>
#include "util.h"


/* The CustomButton class is derived from the QPushButton class, with added stylistic
 * functionality
 */
class CustomButton : public QPushButton {
    Q_OBJECT
public:
    using QPushButton::QPushButton;

protected:
    void enterEvent(QEnterEvent *ev) override;
    void leaveEvent(QEvent *ev) override;
};

#endif // CUSTOMBUTTON_H
