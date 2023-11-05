/* This file contains definitions for the KeyDetectedLabel class (derived from QLabel)
 * Ramsey Nofal, 11/2023
 */

//Standard Qt
#include <QStyle>

//Local
#include "KeyDetectedLabel.h"


KeyDetectedLabel::KeyDetectedLabel(QWidget *parent) : QLabel(parent), StyleWidget(this) {
    set_key_detected(false);
}

void KeyDetectedLabel::set_key_detected(const bool key_detected) {
    set_style_property("detected", key_detected);
    if(key_detected) {
        setText(" API key found!");
    } else {
        setText(" No key detected");
    }
}
