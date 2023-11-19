/* This file contains declarations for the DeviceSelectBox class (derived from QComboBox)
 * Ramsey Nofal, 11/2023
 */


#ifndef DEVICESELECTBOX_H
#define DEVICESELECTBOX_H

//Standard C++
#include <iostream>

//Standard Qt
#include <QComboBox>
#include <QStandardItemModel>
#include <QLabel>
#include <QList>
#include <QTreeView>


class DeviceSelectBox : public QComboBox {
    Q_OBJECT
public:
    QStandardItemModel* model;
    QTreeView *view;

    DeviceSelectBox(QWidget *parent = nullptr);
    std::string get_enabled_device_name();

signals:
    void user_picked_device(const QString& device_name);

private slots:
    void add_items(const std::vector<std::string>& items);
    void add_item_pairs(const std::vector<std::pair<std::string, std::string>>& item_pairs); //<description, name>
    void user_picked_index(const int& index);
    void packet_cap_picked_adapter(const QString& device_name);

private:
    void add_single_device_item(const QString& item_name);
    void add_paired_device_item(const QString& description, const QString& name); //<description, name>

    void create_model();
    void create_view();

    void connect_signals_slots();
};

#endif // DEVICESELECTBOX_H
