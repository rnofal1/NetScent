/* This file contains declarations for the MainWindow class (derived from QMainWindow)
 * Ramsey Nofal, 08/2023
 */


#ifndef MAINWINDOW_H
#define MAINWINDOW_H


/* Standard Qt */
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

/* Local */
#include "ui_mainwindow.h"
#include "Packet.h"
#include "StyleWidget.h"
#include "SharedQueue.h"


/* The MainWindow class is the top-level framework that handles the UI by:
 *
 * (a) maintaining ownership over UI elements
 * (b) signaling certain UI elements to respond to certain user inputs
 * (c) relaying certain signals between UI elements
 */
class MainWindow : public QMainWindow, public StyleWidget {
    Q_OBJECT
public:
    /* Variables */
    bool run_capture;

    /* Functions */
    MainWindow(SharedQueue<Packet*> *packet_queue, QWidget *parent = nullptr);

    //RoT
    ~MainWindow();
    MainWindow(const MainWindow&) = delete;
    MainWindow& operator= (const MainWindow&) = delete;

    Ui::MainWindow* get_ui_pointer();

    void set_widgets_style();

    void connect_signals_slots();

    void update_api_key_status();

    void set_packet_loading_active();
    void set_packet_loading_inactive();

private:
    /* Variables */
    Ui::MainWindow *ui;

    QFuture<void> map_update_thread;

    QFuture<void> map_update_thread_astar; //ASTAR

    SharedQueue<Packet*> *packet_queue;

    QString dummy_api_key;

    /* Functions */
    //Read style sheet from file and return as a string
    QString get_stylesheet();

    QString get_dir_path_from_user();

    //Creates a unique file name based on time, date, etc. for easy searchability
    QString create_record_file_name();

    //Return true on success, false otherwise
    bool write_all_packets_to_file(const QString& path);

    //Creates a new mini-window with a message
    void message_popup(const QString& msg);

    void set_stylesheets();

    /* init_packet_view() gives the packet table view access to the packet queue (which is shared with PacketCap).
     * Starts the packet view's update loop.
     *
     * We could (should?) do this one level up, but this fits better with the idea of MainWindow
     * being the central arbitrator between most of our UI elements.
     */
    void init_packet_view();

    //Remove visible packets and wipe packet array
    void remove_existing_packets();

    void clear_info_pane();

signals:
    void all_packets_added_to_map();

    void new_packet_ready();

    void ui_closed();

    void change_capture_state(const bool& capture_enabled);

    void packets_cleared();

private slots:
    void start_button_clicked();
    void stop_button_clicked();
    void refresh_button_clicked();
    void refresh_button_clicked_astar(); //ASTAR
    void clear_button_clicked();
    void set_api_button_clicked();

    //Saves all existing packets to a file
    void save_to_file();

    //Populates geographical map with location info of existing packets
    void update_map();

    void set_map_loading_active();
    void set_map_loading_inactive();

    void map_update_complete();

    //ASTAR ---- START
    void update_map_astar();

    void set_map_loading_active_astar();
    void set_map_loading_inactive_astar();

    void map_update_complete_astar();
    //ASTAR ---- END

    //Tell the info pane to populate its text with info from a packet
    void update_info_pane(Packet* packet);
};

#endif // MAINWINDOW_H
