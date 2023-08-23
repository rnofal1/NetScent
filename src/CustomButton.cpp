/* This file contains definitions for the CustomButton class (derived from QPushButton)
 * Ramsey Nofal, 08/2023
 */


//Local
#include "CustomButton.h"
#include "util.h"


CustomButton::CustomButton(QWidget *parent) : QPushButton(parent) {
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

//ToDo: reduce code duplication with add_style()/set_style() in classes
void CustomButton::add_style(const std::string& style_name, const std::string& style_val) {
    style_map[style_name] = style_val;
}
void CustomButton::set_style(const std::string& style_name) {
    auto style = style_map.find(style_name);

    if(style != style_map.end()) {
        setStyleSheet(QString::fromStdString(style->second));
    }
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

