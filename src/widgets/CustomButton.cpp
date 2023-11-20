/* This file contains definitions for the CustomButton class (derived from QPushButton)
 * Ramsey Nofal, 08/2023
 */


/* Standard Qt */
#include <QFile>

/* Local */
#include "CustomButton.h"


CustomButton::CustomButton(QWidget *parent) : QPushButton(parent), StyleWidget(this) {
    enable();
}

void CustomButton::enable() {
    setEnabled(true);
    set_style_property("state", "default");
}

void CustomButton::disable() {
    setDisabled(true);
    set_style_property("state", "disabled");
}

void CustomButton::enterEvent(QEnterEvent *ev) {
    Q_UNUSED(ev);
    if(!isEnabled()) {
        return;
    }
    set_style_property("state", "hover");
}

void CustomButton::leaveEvent(QEvent *ev) {
    Q_UNUSED(ev);
    if(!isEnabled()) {
        return;
    }
    set_style_property("state", "default");
}

