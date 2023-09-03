/* This file contains declarations for the PacketScrollArea class (derived from QScrollArea)
 * Ramsey Nofal, 08/2023
 */


#ifndef PACKETSCROLLAREA_H
#define PACKETSCROLLAREA_H


//Standard Qt
#include <QScrollArea>
#include <QScrollBar>

//Local
#include "StyleWidget.h"


class PacketScrollArea : public QScrollArea, public StyleWidget {
    Q_OBJECT
public:
    PacketScrollArea(QWidget *parent = nullptr);
};

#endif // PACKETSCROLLAREA_H
