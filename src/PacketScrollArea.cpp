/* This file contains definitions for the PacketScrollArea class (derived from QScrollArea)
 * Ramsey Nofal, 08/2023
 */


//Local
#include "PacketScrollArea.h"
#include "util.h"

PacketScrollArea::PacketScrollArea(QWidget *parent) : QScrollArea(parent), StyleWidget(this) {
    setAlignment(Qt::AlignTop);
}
