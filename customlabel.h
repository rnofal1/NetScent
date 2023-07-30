#ifndef CUSTOMLABEL_H
#define CUSTOMLABEL_H

#include <QLabel>
#include <QTextBrowser>

class CustomLabel : public QLabel
{
    Q_OBJECT
public:
    CustomLabel(QWidget* infoPaneInit = nullptr, QWidget* parent = nullptr) : QLabel(parent), infoPane(infoPaneInit) { }

protected:
    void enterEvent(QEvent *ev) override
    {
        setStyleSheet("QLabel { background-color : grey; }");
        if(infoPane != nullptr) {
            infoPane->setStyleSheet("QTextBrowser { background-color : blue; }");
        }
    }

    void leaveEvent(QEvent *ev) override
    {
        setStyleSheet("QLabel { background-color : white; }");
        if(infoPane != nullptr) {
            infoPane->setStyleSheet("QTextBrowser { background-color : white; }");
        }
    }

private:
    QWidget* infoPane;

};

#endif // CUSTOMLABEL_H
