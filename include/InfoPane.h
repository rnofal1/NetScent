/* This file contains declarations for the InfoPane class (derived from QTextBrowser)
 * Ramsey Nofal, 08/2023
 */


#ifndef INFOPANE_H
#define INFOPANE_H


//Standard Qt
#include <QTextBrowser>

//Local
#include "StyleWidget.h"


class InfoPane : public QTextBrowser, public StyleWidget {
    Q_OBJECT
public:
    InfoPane(QWidget *parent = nullptr);
};

#endif // INFOPANE_H
