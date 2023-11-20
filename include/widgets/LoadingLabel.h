/* This file contains declarations for the LoadingLabel class (derived from QLabel)
 * Ramsey Nofal, 08/2023
 */


#ifndef LOADINGLABEL_H
#define LOADINGLABEL_H


/* Standard Qt */
#include <QLabel>

/* Local */
#include "StyleWidget.h"


class LoadingLabel : public QLabel, public StyleWidget {
    Q_OBJECT
public:
    /* Functions */
    LoadingLabel(QWidget* parent = nullptr);

    void set_icon();
    void set_movie();
    void stop_movie();

    void set_status_active();
    void set_status_inactive();
};

#endif // LOADINGLABEL_H
