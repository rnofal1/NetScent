#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QThread>
#include <QScrollBar>
#include "customlabel.h"
#include "ui_mainwindow.h"
#include "Packet.h"
#include <iostream>
#include <sstream>
#include <unordered_map>
//#include "PacketCap.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    Ui::MainWindow* getUiPointer();
    bool run_capture;

    static void addPacket(const struct ip& ip_header, const int& packet_num);
    static void addPacket(const struct ip& ip_header, const struct tcphdr& tcp_header, const int& packet_num);
    static void addPacket(const struct ip& ip_header, const struct udphdr& udp_header, const int& packet_num);
    static void addPacket(const struct ip& ip_header, const struct icmp& icmp_header, const int& packet_num);
    static void displayPacket(const std::string& packetText);

private slots:
    void start_button_clicked();
    void stop_button_clicked();

private:
    static Ui::MainWindow *ui;
    static std::unordered_map<int, Packet*> packet_map; //<Packet #, Packet Obj>
};
#endif // MAINWINDOW_H
