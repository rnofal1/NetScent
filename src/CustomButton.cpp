/* This file contains definitions for the CustomButton class (derived from QPushButton)
 * Ramsey Nofal, 08/2023
 */


//Local
#include "CustomButton.h"
#include "util.h"

//ToDo: store style specifications somewhere
void CustomButton::enterEvent(QEnterEvent *ev) {
    Q_UNUSED(ev);

    set_stylesheet_from_json(*this, "startStopButton", "Alt");
}

void CustomButton::leaveEvent(QEvent *ev) {
    Q_UNUSED(ev);

    set_stylesheet_from_json(*this, "startStopButton", "Main");
}
