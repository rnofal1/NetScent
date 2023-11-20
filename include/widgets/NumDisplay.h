/* This file contains declarations for the NumDisplay class (derived from QLineEdit)
 * Ramsey Nofal, 11/2023
 */


#ifndef NUMDISPLAY_H
#define NUMDISPLAY_H


/* Standard Qt */
#include <QLineEdit>

/* Local */
#include "StyleWidget.h"

/* Defines */
#define DEFAULT_DISP_MAX 9999


class NumDisplay : public QLineEdit, public StyleWidget {
    Q_OBJECT
public:
    /* Functions */
    NumDisplay(QWidget *parent = nullptr, int max_num = DEFAULT_DISP_MAX);
    void set_num(int new_num);

private:
    /* Variables */
    int max_num;

private slots:
    void set_num_slot(int new_num);


};

#endif // NUMDISPLAY_H
