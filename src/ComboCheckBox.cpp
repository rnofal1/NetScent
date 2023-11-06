/* This file contains definitions for the ComboCheckBox class (derived from QComboBox)
 * Ramsey Nofal, 08/2023
 */


//Local
#include "ComboCheckBox.h"


ComboCheckBox::ComboCheckBox(QWidget *parent) : QComboBox(parent) {
    QStandardItem* title = new QStandardItem(QString("Filter"));
    model.appendRow(title);

    std::vector<QString> filter_item_names = {"TCP", "UDP", "ICMP"};
    add_filter_items(filter_item_names);

    this->setModel(&model);
}

void ComboCheckBox::add_single_filter_item(const QString& item_name) {
    auto item = new QStandardItem(item_name);
    item->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
    item->setData(Qt::Checked, Qt::CheckStateRole);
    model.appendRow(item);
}
void ComboCheckBox::add_filter_items(const std::vector<QString> item_names) {
    for(auto& name : item_names) {
        add_single_filter_item(name);
    }
}


bool ComboCheckBox::tcp_filter_enabled() {
    return filter_enabled("TCP");
}
bool ComboCheckBox::udp_filter_enabled() {
    return filter_enabled("UDP");
}
bool ComboCheckBox::icmp_filter_enabled() {
    return filter_enabled("ICMP");
}

bool ComboCheckBox::filter_enabled(const QString& name) {
    QStandardItem* filter = find_item(name);
    return filter && filter->checkState() == Qt::Checked;
}

QStandardItem* ComboCheckBox::find_item(const QString &name) {
    QList<QStandardItem *> matched_items = model.findItems(name);

    if(matched_items.empty()) {
        qDebug() << "No matching filter found for: " << name.toStdString() << "\n";
        return nullptr;
    } else if(matched_items.size() > 1) {
        qDebug() << "Too many matching filters found for: " << name.toStdString() << "\n";
        return nullptr;
    } else {
        return matched_items.first();
    }
}
