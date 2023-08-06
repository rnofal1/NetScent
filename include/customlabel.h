/* This file contains declarations for the CustomLabel class (derived from QLabel)
 * Ramsey Nofal, 08/2023
 */


#ifndef CUSTOMLABEL_H
#define CUSTOMLABEL_H


//Standard Qt
#include <QLabel>
#include <QTextBrowser>

//Local
#include "Packet.h"


/* The CustomLabel class is derived from the QLabel class, with the added functionality
 * of storing packet info
 *
 * CustomLabel is used (in tandem with MainWindow) to display packet info
 */
class CustomLabel : public QLabel {
    Q_OBJECT
public:
    CustomLabel(Packet* packet, QTextBrowser* info_pane_init = nullptr, QWidget* parent = nullptr);

protected:
    void enterEvent(QEnterEvent *ev) override;
    void leaveEvent(QEvent *ev) override;

private:
    QTextBrowser* info_pane;
    Packet* packet;
};

#endif // CUSTOMLABEL_H
