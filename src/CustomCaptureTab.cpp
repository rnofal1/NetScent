/* This file contains definitions for the CustomCaptureTab class (derived from QWidget)
 * Ramsey Nofal, 08/2023
 */


//Local
#include "CustomCaptureTab.h"
#include "util.h"

CustomCaptureTab::CustomCaptureTab(QWidget *parent) : QWidget(parent), StyleWidget(this) {
    add_style("Main", get_stylesheet_from_json("captureTab", "Main"));
    set_style("Main");
}
