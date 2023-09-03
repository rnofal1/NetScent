/* This file contains definitions for the CustomButton class (derived from QPushButton)
 * Ramsey Nofal, 08/2023
 */


//Local
#include "CustomButton.h"
#include "util.h"


CustomButton::CustomButton(QWidget *parent) : QPushButton(parent), StyleWidget(this) {
    add_style("Main", get_stylesheet_from_json("startStopButton", "Main"));
    add_style("Alt", get_stylesheet_from_json("startStopButton", "Alt"));
}

void CustomButton::enable() {
    setEnabled(true);
    setToolTip("");
    set_stylesheet_from_json(*this, "startStopButton", "Main");
}

void CustomButton::disable() {
    setDisabled(true);
    setToolTip("Must have at least one packet to save. Cannot save when capture enabled");
    set_stylesheet_from_json(*this, "startStopButton", "Alt");
}

//ToDo: store style specifications somewhere
void CustomButton::enterEvent(QEnterEvent *ev) {
    Q_UNUSED(ev);
    if(!isEnabled()) {
        return;
    }
    set_style("Alt");
}

void CustomButton::leaveEvent(QEvent *ev) {
    Q_UNUSED(ev);
    if(!isEnabled()) {
        return;
    }
    set_style("Main");
}

