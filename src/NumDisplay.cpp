/* This file contains definitions for the NumDisplay class (derived from QLineEdit)
 * Ramsey Nofal, 11/2023
 */


//Local
#include "NumDisplay.h"


NumDisplay::NumDisplay(QWidget *parent, int max_num) : QLineEdit(parent),
                                                       StyleWidget(this),
                                                       max_num(max_num) {
    setReadOnly(true);
    set_num(0);
}

void NumDisplay::set_num(int new_num) {
    if(new_num > max_num) {
        setText(QString::number(max_num) + '+');
    } else {
        setText(QString::number(new_num));
    }
}
