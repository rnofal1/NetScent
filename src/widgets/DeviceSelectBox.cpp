/* This file contains definitions for the DeviceSelectBox class (derived from QComboBox)
 * Ramsey Nofal, 11/2023
 */


//Standard Qt
#include <QTreeView>
#include <QHeaderView>

//Local
#include "DeviceSelectBox.h"


//ToDo: There is some redundancy between this class and PacketFilterCheckBox
DeviceSelectBox::DeviceSelectBox(QWidget *parent) : QComboBox(parent) {
    create_model();
    create_view();
    connect_signals_slots();
}

void DeviceSelectBox::create_model() {
    model =  new QStandardItemModel();
    this->setModel(model);
}
void DeviceSelectBox::create_view() {
    view = new QTreeView(this);
    view->setHeaderHidden(true);
    view->setSelectionMode(QAbstractItemView::SingleSelection);
    view->setRootIsDecorated(false);
    this->setView(view);
}

void DeviceSelectBox::connect_signals_slots() {
    connect(this, SIGNAL(activated(int)), this, SLOT(user_picked_index(int)));
}

void DeviceSelectBox::add_single_device_item(const QString& item_name) {
    auto item = new QStandardItem(item_name);
    model->appendRow(item);
}

void DeviceSelectBox::add_items(const std::vector<std::string>& items) {
    for(auto& item : items) {
        add_single_device_item(QString::fromStdString(item));
    }
}

void DeviceSelectBox::add_paired_device_item(const QString& description, const QString& name) {
    auto desc_item = new QStandardItem(description);
    auto name_item = new QStandardItem(name);
    name_item->setEnabled(false);
    desc_item->appendColumn({name_item});
    model->appendRow(desc_item);
}

void DeviceSelectBox::add_item_pairs(const std::vector<std::pair<std::string, std::string>>& item_pairs) {//<description, name>
    for(auto& pair : item_pairs) {
        add_paired_device_item(QString::fromStdString(pair.first), QString::fromStdString(pair.second));
    }
    view->expandAll();
}

void DeviceSelectBox::user_picked_index(const int& index) {
    QStandardItem* device_desc_item = model->item(index, 0); //Holds the human-readable description
    QStandardItem* device_name_item = device_desc_item->child(0, 0); //Holds the actual name
    QString device_name = device_name_item->text();
    qInfo() << "User picked network adapter at index: " << index << " With Name: " << device_name;
    emit user_picked_device(device_name);
}

void DeviceSelectBox::packet_cap_picked_adapter(const QString& device_name) {
    qInfo() << "PacketCap selected device: " << device_name;
    for (int row = 0; row < model->rowCount(); ++row) {
        QStandardItem* device_desc_item = model->item(row, 0); //Holds the human-readable description
        QStandardItem* device_name_item = device_desc_item->child(0, 0); //Holds the actual name

        if(device_name_item->text() == device_name) {
            setCurrentIndex(row);
        }
    }
}