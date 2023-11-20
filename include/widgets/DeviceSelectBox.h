/* This file contains declarations for the DeviceSelectBox class (derived from QComboBox)
 * Ramsey Nofal, 11/2023
 */


#ifndef DEVICESELECTBOX_H
#define DEVICESELECTBOX_H

/* Standard C/C++ */
#include <iostream>

/* Standard Qt */
#include <QComboBox>
#include <QStandardItemModel>
#include <QLabel>
#include <QList>
#include <QTreeView>


class DeviceSelectBox : public QComboBox {
    Q_OBJECT
public:
    /* Variables */
    QStandardItemModel* model;
    QTreeView *view;

    /* Functions */
    DeviceSelectBox(QWidget *parent = nullptr);

private:
    /* Functions */
    void add_single_device_item(const QString& item_name);

    //Network devices have a human-readable description and an actual name
    void add_paired_device_item(const QString& description, const QString& name);

    void create_model();
    void create_view();

    void connect_signals_slots();

signals:
    //Tell anyone listening that the user chose device with device_name
    void user_picked_device(const QString& device_name);

private slots:
    void add_items(const std::vector<std::string>& items);

    //Pairs represent: <description, name> (Network devices have a human-readable description and an actual name)
    void add_item_pairs(const std::vector<std::pair<std::string, std::string>>& item_pairs);

    void user_picked_index(const int& index);

    //If PacketCap picked a device/adapter (e.g. on startup), set that device as active
    void packet_cap_picked_adapter(const QString& device_name);
};

#endif // DEVICESELECTBOX_H
