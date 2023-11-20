/* This file contains declarations for the PacketFilterCheckBox class (derived from QComboBox)
 * Ramsey Nofal, 08/2023
 */


#ifndef PACKETFILTERCHECKBOX_H
#define PACKETFILTERCHECKBOX_H


/* Standard C/C++ */
#include <iostream>

/* Standard Qt */
#include <QComboBox>
#include <QStandardItemModel>
#include <QLabel>
#include <QList>


class PacketFilterCheckBox : public QComboBox {
    Q_OBJECT
public:
    /* Variables */
    QStandardItemModel model;

    /* Functions */
    PacketFilterCheckBox(QWidget *parent = nullptr);

    //Filter checking has two interfaces
    bool tcp_filter_enabled();
    bool udp_filter_enabled();
    bool icmp_filter_enabled();

    //Filter checking has two interfaces
    bool filter_enabled(const QString& name);

private:
    /* Functions */
    QStandardItem* find_item(const QString &name);

    void create_title(const QString& title);
    void create_dropdown_items(const std::vector<QString>& dropdown_item_names);
    void create_single_dropdown_item(const QString& item_name);
};

#endif // PACKETFILTERCHECKBOX_H
