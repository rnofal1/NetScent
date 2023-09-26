/* This file contains definitions for the ComboCheckBox class (derived from QComboBox)
 * Ramsey Nofal, 08/2023
 */


//Local
#include "ComboCheckBox.h"


ComboCheckBox::ComboCheckBox(QWidget *parent) : QComboBox(parent) {
    QStandardItem* title = new QStandardItem(QString("Filter"));
    model.appendRow(title);

    QStandardItem* tcp = new QStandardItem(QString("TCP"));
    tcp->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
    tcp->setData(Qt::Checked, Qt::CheckStateRole);
    model.appendRow(tcp);

    QStandardItem* udp = new QStandardItem(QString("UDP"));
    udp->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
    udp->setData(Qt::Checked, Qt::CheckStateRole);
    model.appendRow(udp);

    QStandardItem* icmp = new QStandardItem(QString("ICMP"));
    icmp->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
    icmp->setData(Qt::Checked, Qt::CheckStateRole);
    model.appendRow(icmp);

    this->setModel(&model);
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
        std::cout << "No matching filter found for: " << name.toStdString() << "\n";
        return nullptr;
    } else if(matched_items.size() > 1) {
        std::cout << "Too many matching filters found for: " << name.toStdString() << "\n";
        return nullptr;
    } else {
        return matched_items.first();
    }
}
