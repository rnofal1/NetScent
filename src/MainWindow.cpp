/* This file contains definitions for the MainWindow class (derived from QMainWindow)
 * Ramsey Nofal, 08/2023
 */


//Standard Qt
#include <QtConcurrent>

//Local
#include "MainWindow.h"
#include "util.h"
#include "LoadingLabel.h"


MainWindow::MainWindow(SharedQueue<Packet*> *packet_queue, QWidget *parent)
                        : QMainWindow(parent),
                        StyleWidget(this),
                        run_capture(false),
                        closed(false),
                        clear_packets(false),
                        packet_queue(packet_queue) {
    ui = new Ui::MainWindow();
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);

    set_widgets_style();

    connect_signals_slots();

    init_packet_view();
}

MainWindow::~MainWindow() {
    qDebug() << "In MainWindow destructor";
    emit ui_closed();
    emit change_capture_state(false);
    map_update_thread.waitForFinished();
    if(ui != nullptr) {
        delete ui;
    }
    qDebug() << "Leaving MainWindow destructor";
}

Ui::MainWindow* MainWindow::get_ui_pointer() {
    return this->ui;
}

QString MainWindow::get_stylesheet() {
    QFile styleFile(STYLE_FILE);
    styleFile.open(QFile::ReadOnly);
    QString style(styleFile.readAll());
    styleFile.close();
    return style;
}

//ToDo: all children should inherit the MainWindow stylesheet, but it seems like they don't (this shouldn't be necessary)
void MainWindow::set_child_stylesheets() {
    QString style = get_stylesheet();
    setStyleSheet(style);
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

void MainWindow::set_widgets_style() {
    //ApiLinkLabel Style
    ui->apiLinkLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
    ui->apiLinkLabel->setToolTip("Follow this link, create an account, and paste your API key at"
                                 " the right to unlock IP geolocation");

    //Tabs Style
    ui->tabWidget->setCurrentIndex(0);

    //Disable certain buttons
    ui->saveButton->disable();

    update_api_key_status();

    set_child_stylesheets();
}

void MainWindow::connect_signals_slots() {
    //Buttons
    connect(ui->startButton, SIGNAL(clicked()), this, SLOT(start_button_clicked()));
    connect(ui->stopButton, SIGNAL(clicked()), this, SLOT(stop_button_clicked()));
    connect(ui->setApiKeyButton, SIGNAL(clicked()), this, SLOT(set_api_button_clicked()));
    connect(ui->packetClearButton, SIGNAL(clicked()), this, SLOT(clear_button_clicked()));
    connect(ui->packetClearButton, SIGNAL(clicked()), ui->packetTableView, SLOT(clear_view()));
    connect(ui->saveButton, SIGNAL(clicked()), this, SLOT(save_to_file()));
    connect(ui->mapRefreshButton, SIGNAL(clicked()), this, SLOT(refresh_button_clicked()));

    //Packet filters
    connect(&ui->filterBox->model, SIGNAL(itemChanged(QStandardItem*)), ui->packetTableView, SLOT(filters_changed(QStandardItem*)));

    //Device select
    connect(ui->deviceSelect, SIGNAL(activated(int)), this, SLOT(clear_button_clicked()));
    connect(ui->deviceSelect, SIGNAL(activated(int)), ui->packetTableView, SLOT(clear_view()));


    //Packet Table View
    connect(this, SIGNAL(ui_closed()), ui->packetTableView, SLOT(set_ui_closed()));
    connect(ui->packetTableView, SIGNAL(send_clicked_packet(Packet*)), this, SLOT(update_info_pane(Packet*)));
    connect(ui->packetTableView, SIGNAL(num_packets_displayed_changed(int)), ui->numPacketsDisplay, SLOT(set_num_slot(int)));
    connect(ui->packetTableView, SIGNAL(all_packets_added_to_map()), this, SLOT(map_update_complete()));
}

void MainWindow::init_packet_view() {
    ui->packetTableView->set_packet_queue(packet_queue);
    ui->packetTableView->start_queue_poll();
}

// ToDo: consider condensing these
void MainWindow::set_packet_loading_active() {
    auto packetLoadingLabel = ui->packetLoadingLabel;
    packetLoadingLabel->set_status_active();
}
void MainWindow::set_packet_loading_inactive() {
    auto packetLoadingLabel = ui->packetLoadingLabel;
    packetLoadingLabel->set_status_inactive();
}
void MainWindow::set_map_loading_active() {
    auto mapLoadingLabel = ui->mapLoadingLabel;
    mapLoadingLabel->set_status_active();
}
void MainWindow::set_map_loading_inactive() {
    auto mapLoadingLabel = ui->mapLoadingLabel;
    mapLoadingLabel->set_status_inactive();
}

 //Continuously add elements from shared Packet queue
void MainWindow::poll_queue() {
    while(!closed) {
        if(!packet_queue->is_empty()) {
            emit new_packet_ready();
        }
        QThread::msleep(200); //Prevents GUI from being inundated with too many packets
    }
    qDebug() << "Polling halted";
}

void MainWindow::closeEvent(QCloseEvent *ev) {
    Q_UNUSED(ev);
    closed = true;
}

void MainWindow::start_button_clicked() {
    ui->saveButton->disable();
    set_packet_loading_active();
    run_capture = true;
    emit change_capture_state(run_capture);
}
void MainWindow::stop_button_clicked() {
    set_packet_loading_inactive();
    run_capture = false;
    emit change_capture_state(run_capture);

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
    stop_button_clicked();
    clear_packets = true;
    clear_info_pane();
    delete_packets();

    ui->tabWidget->clear_map();
    ui->numPacketsDisplay->set_num(0);
    ui->saveButton->disable();
}

//ToDo: URGENT Handle api key(s) in a more sensible way
void MainWindow::set_api_button_clicked() {
    std::string input_key = ui->apiKeyText->text().toStdString();

    if(input_key != dummy_api_key) {
        std::fstream api_key_file(API_KEY_FILE, std::fstream::out | std::fstream::trunc);
        if(api_key_file) {
            api_key_file << input_key << std::endl;
            api_key_file.close();
        } else {
            qDebug() << "API Key file write error";
        }
    }

    update_api_key_status();
}

//ToDo: URGENT Handle api key(s) in a more sensible way
//ToDo: just use the function in util
void MainWindow::update_api_key_status() {
    std::fstream api_key_file(API_KEY_FILE, std::fstream::in | std::fstream::app);
    std::string key;

    if(api_key_file) {
        std::getline(api_key_file, key);
        api_key_file.close();
    } else {
        qDebug() << "API Key file read error";
    }

    if(key.length() > 0) {
        dummy_api_key = std::string(key.length(), '*');
        ui->apiKeyText->setText(QString::fromStdString(dummy_api_key));
        ui->keyDetectedLabel->set_key_detected(true);
    } else {
        ui->keyDetectedLabel->set_key_detected(false);
    }
}

//ToDo: give user more control over save file
void MainWindow::save_to_file() {
    std::string initial_path = get_dir_path_from_user();
    if(initial_path == "") {
        return;
    }
    std::string record_file_name = create_record_file_name();
    std::string full_path = initial_path + "/" + record_file_name;

    bool write_success = ui->packetTableView->write_packets_to_file(full_path);
    if(write_success) {
        message_popup("Packet record saved as: " + full_path);
    } else {
        message_popup("ERROR: record not saved");
    }
}

void MainWindow::delete_packets() {
    std::for_each(packets.begin(), packets.end(), delete_ptr());
    packets.clear();
}

void MainWindow::message_popup(const std::string& msg) {
    QMessageBox msgBox;

    msgBox.setText(QString::fromStdString(msg));
    msgBox.exec();
}

void MainWindow::refresh_button_clicked() {
    stop_button_clicked();
    ui->packetClearButton->disable();
    ui->mapClearButton->disable();
    ui->startButton->disable();
    set_map_loading_active();
//    map_update_thread = QtConcurrent::run([this] {update_map();});
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

std::string MainWindow::get_dir_path_from_user() {
    std::string folder_path = get_cwd() + "/Records";

    try {
        std::filesystem::create_directory(folder_path);
    } catch(...){
        auto err_msg = "ERROR: Could not create directory at path: " + folder_path;
        qDebug() << err_msg;
        message_popup(err_msg);
        return "";
    }

    QString path = QFileDialog::getExistingDirectory(this,
                                                         tr("Choose Save Folder"),
                                                         QString::fromStdString(folder_path),
                                                         QFileDialog::ShowDirsOnly
                                                         );
    return path.toStdString();
}

std::string MainWindow::create_record_file_name() {
    std::time_t time_saved = std::time(nullptr);

    std::string record_path_preset ="PacketRecord_";
    record_path_preset += std::asctime(std::localtime(&time_saved));
    record_path_preset += ".txt";
    record_path_preset.erase(std::remove(record_path_preset.begin(), record_path_preset.end(), '\n'), record_path_preset.end());
    std::replace(record_path_preset.begin(), record_path_preset.end(), ' ', '_');
    std::replace(record_path_preset.begin(), record_path_preset.end(), ':', '#');

    return record_path_preset;
}

void MainWindow::update_info_pane(Packet* packet) {
    ui->infoPane->set_active();
    ui->infoPane->update_text(packet);
}
