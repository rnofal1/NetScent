/* This file contains declarations for the ComboCheckBox class (derived from QComboBox)
 * Ramsey Nofal, 08/2023
 */


#ifndef COMBOCHECKBOX_H
#define COMBOCHECKBOX_H


//Standard C++
#include <iostream>

//Standard Qt
#include <QComboBox>
#include <QStandardItemModel>
#include <QLabel>
#include <QList>


class ComboCheckBox : public QComboBox {
    Q_OBJECT
public:
    QStandardItemModel model;

    ComboCheckBox(QWidget *parent = nullptr);

    //Filter checking has two interfaces
    bool tcp_filter_enabled();
    bool udp_filter_enabled();
    bool icmp_filter_enabled();

    //Filter checking has two interfaces
    bool filter_enabled(const QString& name);

private:
    QStandardItem* find_item(const QString &name);

    void add_single_filter_item(const QString& item_name);
    void add_filter_items(const std::vector<QString> item_names);
};

#endif // COMBOCHECKBOX_H
