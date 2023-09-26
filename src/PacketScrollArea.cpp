/* This file contains definitions for the PacketScrollArea class (derived from QScrollArea)
 * Ramsey Nofal, 08/2023
 */


//Local
#include "PacketScrollArea.h"
#include "util.h"

PacketScrollArea::PacketScrollArea(QWidget *parent) : QScrollArea(parent), StyleWidget(this) {
    add_style("Main", get_stylesheet_from_json("scrollArea", "Main"));
    set_style("Main");
    setAlignment(Qt::AlignTop);

    set_stylesheet_from_json(*verticalScrollBar(), "scrollBar", "Main");
    set_stylesheet_from_json(*horizontalScrollBar(), "scrollBar", "Main");
}
