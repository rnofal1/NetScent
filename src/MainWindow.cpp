/* This file contains definitions for the MainWindow class (derived from QMainWindow)
 * Ramsey Nofal, 08/2023
 */


/* Standard Qt */
#include <QtConcurrent>
#include <QDir>

/* Local */
#include "MainWindow.h"
#include "util.h"
#include "LoadingLabel.h"


MainWindow::MainWindow(SharedQueue<Packet*> *packet_queue, QWidget *parent)
                        : QMainWindow(parent),
                        StyleWidget(this),
                        run_capture(false),
                        packet_queue(packet_queue) {
    ui = new Ui::MainWindow();
    ui->setupUi(this);

    //Force destructor to run when user closes window
    setAttribute(Qt::WA_DeleteOnClose);

    set_widgets_style();

    connect_signals_slots();

    init_packet_view();
}

MainWindow::~MainWindow() {
    emit ui_closed();
    emit change_capture_state(false);

    map_update_thread.waitForFinished();

    if(ui != nullptr) {
        delete ui;
    }
}

Ui::MainWindow* MainWindow::get_ui_pointer() {
    return this->ui;
}

//Read style sheet from file and return as a string
QString MainWindow::get_stylesheet() {
    return read_from_file(STYLE_FILE_PATH);
}

void MainWindow::set_stylesheets() {
    QString style = get_stylesheet();

    //Set own style sheet
    setStyleSheet(style);

    //Set child style sheets
    //ToDo: all children should inherit the MainWindow stylesheet, but it seems like they don't (this shouldn't be necessary)
    QList<QWidget*> children = findChildren<QWidget*>();
    for(auto& child : children) {
        auto child_style = dynamic_cast<StyleWidget*>(child);
        if(child_style != nullptr) {
            child_style->reset_style();
        } else if(child != nullptr) {
            child->setStyleSheet(style);
        }
    }

    this->ensurePolished();
}

//ToDo: some of this can be set in a style sheet (but should it?)
void MainWindow::set_widgets_style() {
    //ApiLinkLabel Style
    ui->apiLinkLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
    ui->apiLinkLabel->setToolTip("Follow this link, create an account, and paste your API key at"
                                 " the right to unlock IP geolocation");

    //Make sure we start on the 0th/leftmost tab
    ui->tabWidget->setCurrentIndex(0);

    //Disable certain button(s)
    ui->saveButton->disable();

    //Detect if there exists an api key
    update_api_key_status();

    set_stylesheets();
}

/* Some of these connections can be set within the children themselves (or split into separate functions),
 * but it seems easier to make this (mostly) centralized.
 *
 * ToDo: a little messy
 */
void MainWindow::connect_signals_slots() {
    //Buttons --> X
    connect(ui->startButton, SIGNAL(clicked()), this, SLOT(start_button_clicked()));
    connect(ui->stopButton, SIGNAL(clicked()), this, SLOT(stop_button_clicked()));
    connect(ui->setApiKeyButton, SIGNAL(clicked()), this, SLOT(set_api_button_clicked()));
    connect(ui->packetClearButton, SIGNAL(clicked()), this, SLOT(clear_button_clicked()));
    connect(ui->packetClearButton, SIGNAL(clicked()), ui->packetTableView, SLOT(clear_view()));
    connect(ui->saveButton, SIGNAL(clicked()), this, SLOT(save_to_file()));
    connect(ui->mapRefreshButton, SIGNAL(clicked()), this, SLOT(refresh_button_clicked()));

    //Packet filter dropdown --> X
    connect(&ui->filterBox->model, SIGNAL(itemChanged(QStandardItem*)), ui->packetTableView, SLOT(filters_changed(QStandardItem*)));

    //Device select dropdown --> X
    connect(ui->deviceSelect, SIGNAL(activated(int)), this, SLOT(clear_button_clicked()));
    connect(ui->deviceSelect, SIGNAL(activated(int)), ui->packetTableView, SLOT(clear_view()));


    //Packet table view -- > X
    connect(ui->packetTableView, SIGNAL(send_clicked_packet(Packet*)), this, SLOT(update_info_pane(Packet*)));
    connect(ui->packetTableView, SIGNAL(num_packets_displayed_changed(int)), ui->numPacketsDisplay, SLOT(set_num_slot(int)));
    connect(ui->packetTableView, SIGNAL(all_packets_added_to_map()), this, SLOT(map_update_complete()));

    //MainWindow (this) --> X
    connect(this, SIGNAL(ui_closed()), ui->packetTableView, SLOT(set_ui_closed()));
}

/* Gives the packet table view access to the packet queue (which is shared with PacketCap).
 * Starts the packet view's update loop.
 *
 * We could (should?) do this one level up, but this fits better with the idea of MainWindow
 * being the central arbitrator between most of our UI elements.
 */
void MainWindow::init_packet_view() {
    ui->packetTableView->set_packet_queue(packet_queue);
    ui->packetTableView->start_queue_poll();
}

// ToDo: consider condensing these
void MainWindow::set_packet_loading_active() {
    ui->packetLoadingLabel->set_status_active();
}
void MainWindow::set_packet_loading_inactive() {
    ui->packetLoadingLabel->set_status_inactive();
}
void MainWindow::set_map_loading_active() {
    ui->mapLoadingLabel->set_status_active();
}
void MainWindow::set_map_loading_inactive() {
    ui->mapLoadingLabel->set_status_inactive();
}

void MainWindow::start_button_clicked() {
    //Disable saving if packets are being read
    ui->saveButton->disable();

    set_packet_loading_active();

    run_capture = true;
    emit change_capture_state(run_capture);
}
void MainWindow::stop_button_clicked() {
    set_packet_loading_inactive();

    run_capture = false;
    emit change_capture_state(run_capture);

    //Enable saving only if we have packets available
    if(ui->packetTableView->get_num_packets_displayed() > 0) { //ToDo: a little inefficient
        ui->saveButton->enable();
    }
}

void MainWindow::clear_button_clicked() {
    remove_existing_packets();
}

void MainWindow::clear_info_pane() {
    ui->infoPane->clear();
    ui->infoPane->set_inactive();
}
void MainWindow::remove_existing_packets() {
    //We don't want to read packets while we're clearing existing packets (personal pref)
    stop_button_clicked();

    //Tell anyone listening that we have cleared the existing packets
    emit packets_cleared();

    clear_info_pane();

    ui->tabWidget->clear_map();

    ui->numPacketsDisplay->set_num(0);

    ui->saveButton->disable();
}

void MainWindow::set_api_button_clicked() {
    QString input_key = ui->apiKeyText->text();

    if(input_key != dummy_api_key) {
        if(!write_to_file(API_KEY_FILE_PATH, input_key, QFile::Truncate)) {
            qDebug() << "API Key file write error";
        }
    }

    update_api_key_status();
}

//ToDo: URGENT Handle api key(s) in a more sensible way
//ToDo: just use the function in util
void MainWindow::update_api_key_status() {
    QString key = read_from_file(API_KEY_FILE_PATH);
    qDebug() << "Read key: " << key;

    //Hide key with '*' characters, set key detected label
    if(key.length() > 0) {
        dummy_api_key = QString(key.length(), '*');
        ui->apiKeyText->setText(dummy_api_key);
        ui->keyDetectedLabel->set_key_detected(true);
    } else {
        ui->keyDetectedLabel->set_key_detected(false);
    }
}

//ToDo: give user more control over save file
void MainWindow::save_to_file() {
    QString initial_path = get_dir_path_from_user();
    if(initial_path == "") {
        return;
    }
    QString record_file_name = create_record_file_name();
    QString full_path = initial_path + "/" + record_file_name;

    bool write_success = ui->packetTableView->write_packets_to_file(full_path);
    if(write_success) {
        message_popup("Packet record saved as: " + full_path);
    } else {
        message_popup("ERROR: record not saved");
    }
}

void MainWindow::message_popup(const QString& msg) {
    QMessageBox msgBox;

    msgBox.setText(msg);
    msgBox.exec();
}

void MainWindow::refresh_button_clicked() {
    stop_button_clicked();

    ui->packetClearButton->disable();
    ui->mapClearButton->disable();
    ui->startButton->disable();

    set_map_loading_active();
    update_map();
}

void MainWindow::update_map() {
    map_update_thread = QtConcurrent::run([this] {ui->packetTableView->update_packet_map(ui->mapTab);});
}

void MainWindow::map_update_complete() {
    set_map_loading_inactive();

    ui->packetClearButton->enable();
    ui->mapClearButton->enable();
    ui->startButton->enable();
}

QString MainWindow::get_dir_path_from_user() {
    QString folder_path = get_cwd() + "/Records";
    if(!QDir(folder_path).exists()) {
        QDir().mkdir(folder_path);
    }

    QString path = QFileDialog::getExistingDirectory(this,
                                                         tr("Choose Save Folder"),
                                                         folder_path,
                                                         QFileDialog::ShowDirsOnly
                                                         );
    return path;
}

QString MainWindow::create_record_file_name() {
    std::time_t time_saved = std::time(nullptr);

    std::string record_path_preset ="PacketRecord_";
    record_path_preset += std::asctime(std::localtime(&time_saved));
    record_path_preset += ".txt";
    record_path_preset.erase(std::remove(record_path_preset.begin(), record_path_preset.end(), '\n'), record_path_preset.end());
    std::replace(record_path_preset.begin(), record_path_preset.end(), ' ', '_');
    std::replace(record_path_preset.begin(), record_path_preset.end(), ':', '#');

    return QString::fromStdString(record_path_preset);
}

void MainWindow::update_info_pane(Packet* packet) {
    ui->infoPane->set_active();
    ui->infoPane->update_text(packet);
}
