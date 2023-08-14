/* This file contains definitions for the CustomButton class (derived from QPushButton)
 * Ramsey Nofal, 08/2023
 */


//Local
#include "CustomButton.h"


//CustomButton::CustomButton() {}

//ToDo: store style specifications somewhere
void CustomButton::enterEvent(QEnterEvent *ev) {
    Q_UNUSED(ev);

    setStyleSheet("background-color : grey");
    setStyleSheet("color: white");
}

void CustomButton::leaveEvent(QEvent *ev) {
    Q_UNUSED(ev);

    setStyleSheet("background-color: rgb(87, 227, 137); color: black");
}
