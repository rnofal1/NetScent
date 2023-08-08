/* This file contains declarations for the MainWindow class (derived from QMainWindow)
 * Ramsey Nofal, 08/2023
 */


#ifndef MAINWINDOW_H
#define MAINWINDOW_H

//Standard C/C++
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <stdlib.h>

//Standard Qt
#include <QMainWindow>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QThread>
#include <QScrollBar>
#include <QLineEdit>
#include <QMovie>
#include <QPixmap>

//Local
#include "ui_mainwindow.h"
#include "CustomLabel.h"


/* The MainWindow class is derived from the QMainWindow class, with the added functionality
 * of storing packet info
 *
 * MainWindow is used (in tandem with CustomLabel) to display packet info
 */
class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    bool run_capture;
    bool closed;

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    
    Ui::MainWindow* get_ui_pointer();

    void set_widgets_style();

    void connect_buttons();


    static void add_packet(const struct ip& ip_header, const int& packet_num);
    static void add_packet(const struct ip& ip_header, const struct tcphdr& tcp_header, const int& packet_num);
    static void add_packet(const struct ip& ip_header, const struct udphdr& udp_header, const int& packet_num);
    static void add_packet(const struct ip& ip_header, const struct icmp& icmp_header, const int& packet_num);

    static void display_packet(const std::string& packetText, Packet* packet);

    static void add_line();

    static void update_api_key_status();

    static void set_status_label_active();
    static void set_status_label_inactive();

protected:
    void closeEvent(QCloseEvent *ev) override;

private slots:
    void start_button_clicked();
    void stop_button_clicked();

    void set_api_button_clicked();

private:
    static Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
