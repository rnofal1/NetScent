#ifndef CUSTOMLABEL_H
#define CUSTOMLABEL_H

#include <QLabel>
#include <QTextBrowser>
#include "Packet.h"
//#include <curl/curl.h>
//#include <nlohmann/json.hpp>
//#include <cstdlib>

class CustomLabel : public QLabel
{
    Q_OBJECT
public:
    CustomLabel(Packet* packet, QTextBrowser* infoPaneInit = nullptr, QWidget* parent = nullptr);

protected:
    void enterEvent(QEnterEvent *ev) override;
    void leaveEvent(QEvent *ev) override;

private:
    QTextBrowser* infoPane;
    Packet* packet;

};

#endif // CUSTOMLABEL_H
