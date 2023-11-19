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
#include "SharedQueue.h"


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
    bool clear_packets;

    MainWindow(SharedQueue<Packet*> *packet_queue, QWidget *parent = nullptr);
    ~MainWindow();
    
    Ui::MainWindow* get_ui_pointer();

    void set_widgets_style();

    void connect_signals_slots();

    void update_api_key_status();

    void set_packet_loading_active();
    void set_packet_loading_inactive();

protected:
    void closeEvent(QCloseEvent *ev) override;

private slots:
    void start_button_clicked();
    void stop_button_clicked();
    void refresh_button_clicked();
    void clear_button_clicked();

    void set_api_button_clicked();

    void save_to_file();

    void update_map();

    void set_map_loading_active();
    void set_map_loading_inactive();

    void map_update_complete();

    void update_info_pane(Packet* packet);

signals:
    void all_packets_added_to_map();
    void new_packet_ready();
    void ui_closed();
    void change_capture_state(const bool& capture_enabled);

private:
    Ui::MainWindow *ui;

    std::vector<Packet* > packets;

    QFuture<void> map_update_thread;

    SharedQueue<Packet*> *packet_queue;

    std::string dummy_api_key;

    bool window_open;

    QString get_stylesheet();
    std::string get_dir_path_from_user();
    std::string create_record_file_name();

    //Return true on success, false otherwise
    bool write_all_packets_to_file(const std::string& path);

    void delete_packets();

    void message_popup(const std::string& msg);

    void set_child_stylesheets();

    void poll_queue(); //Continuously add elements from shared Packet queue

    void init_packet_view();

    //Remove visible packets and wipe packet array
    void remove_existing_packets();

    void clear_info_pane();
};

#endif // MAINWINDOW_H
