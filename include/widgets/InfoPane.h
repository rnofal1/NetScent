/* This file contains declarations for the InfoPane class (derived from QTextBrowser)
 * Ramsey Nofal, 08/2023
 */


#ifndef INFOPANE_H
#define INFOPANE_H


/* Standard Qt */
#include <QTextBrowser>

/* Local */
#include "StyleWidget.h"
#include "Packet.h"


class InfoPane : public QTextBrowser, public StyleWidget {
    Q_OBJECT
public:
    /* Functions */
    InfoPane(QWidget *parent = nullptr);
    void set_active();
    void set_inactive();

    void update_text(Packet* packet);
};

#endif // INFOPANE_H
