/* This file contains declarations for the MainWindow class (derived from QMainWindow)
 * Ramsey Nofal, 08/2023
 */


#ifndef MAINWINDOW_H
#define MAINWINDOW_H


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
#include <QFileDialog>
#include <QLCDNumber>
#include <QDebug>

//Local
#include "ui_mainwindow.h"
#include "Packet.h"
#include "StyleWidget.h"

//Defines
#define PACKET_LIMIT 999

/* The MainWindow class is derived from the QMainWindow class, with the added functionality
 * of storing packet info
 *
 * MainWindow is used (in tandem with CustomLabel) to display packet info
 */
class MainWindow : public QMainWindow, public StyleWidget {
    Q_OBJECT
public:
    std::shared_ptr<std::map<std::string, std::string>> geo_cache_map;
    bool run_capture;
    bool closed;
    static bool clear_packets;

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    
    Ui::MainWindow* get_ui_pointer();

    void set_widgets_style();

    void connect_buttons();

    void update_api_key_status();

    void set_packet_loading_active();
    void set_packet_loading_inactive();

    static void add_packet(const struct ip& ip_header, const int& packet_num);
    static void add_packet(const struct ip& ip_header, const struct tcphdr& tcp_header, const int& packet_num);
    static void add_packet(const struct ip& ip_header, const struct udphdr& udp_header, const int& packet_num);
    static void add_packet(const struct ip& ip_header, const struct icmp& icmp_header, const int& packet_num);

    static void display_packet(Packet* packet);

    static void add_line();

protected:
    void closeEvent(QCloseEvent *ev) override;

private slots:
    void start_button_clicked();
    void stop_button_clicked();
    void refresh_button_clicked();

    //Only hide packets while retaining packet array
    void clear_packet_display();

    //Remove visible packets and wipe packet array
    void remove_existing_packets();

    void set_api_button_clicked();

    void refresh_packet_window();

    void save_to_file();

    void update_map();

    void set_map_loading_active();
    void set_map_loading_inactive();

    void map_update_complete();

signals:
    void all_packets_added_to_map();

private:
    static Ui::MainWindow *ui;
    static std::vector<Packet* > packets;
    std::string dummy_api_key;

    QString get_stylesheet();
    std::string get_dir_path_from_user();
    std::string create_record_file_name();

    //Return true on success, false otherwise
    bool write_all_packets_to_file(const std::string& path);

    //Add packets to window based on filter settings
    void add_valid_packets();

    void delete_packets();

    void message_popup(const std::string& msg);

    void set_child_stylesheets();
};

#endif // MAINWINDOW_H
