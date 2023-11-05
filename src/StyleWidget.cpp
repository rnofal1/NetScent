/* This file contains definitions for the StyleWidget class
 * Ramsey Nofal, 08/2023
 */


//Standard Qt
#include <QFile>

//Local
#include "StyleWidget.h"


StyleWidget::StyleWidget(QWidget *widget) : this_widget(widget) {}

void StyleWidget::reset_style() {
    QFile styleFile( "style/style.qss" );
    styleFile.open(QFile::ReadOnly);
    QString style(styleFile.readAll());
    this_widget->setStyleSheet(style);
    this_widget->ensurePolished();
}

void StyleWidget::set_style_property(const char* name, const QVariant &value) {
    this_widget->setProperty(name, value);
    reset_style();
}
