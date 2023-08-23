/* This file contains definitions for the MainWindow class (derived from QMainWindow)
 * Ramsey Nofal, 08/2023
 */


//Local
#include "MainWindow.h"
#include "PacketLabel.h"
#include "util.h"

//Static variable definitions
Ui::MainWindow *MainWindow::ui = nullptr;
std::vector<Packet* > MainWindow::packets;
bool MainWindow::clear_packets = false;

MainWindow::MainWindow(QWidget *parent)
                       : QMainWindow(parent),
                        run_capture(false),
                        closed(false){
    ui = new Ui::MainWindow();
    ui->setupUi(this);

    set_widgets_style();
    connect_buttons();
}

MainWindow::~MainWindow() {
    //ToDo: consider changing movie ownership
    if(ui->statusLabel->movie()) {
        delete ui->statusLabel->movie();
    }

    delete_packets();

    delete ui;
}

Ui::MainWindow* MainWindow::get_ui_pointer() {
    return this->ui;
}

//ToDo: consider adding stylesheet setting functions within derived UI/GUI classes
void MainWindow::set_widgets_style() {
    //ScrollArea Style
    set_stylesheet_from_json(*ui->scrollArea, "scrollArea", "Main");
    set_stylesheet_from_json(*ui->scrollArea->verticalScrollBar(), "scrollBar", "Main");
    set_stylesheet_from_json(*ui->scrollArea->horizontalScrollBar(), "scrollBar", "Main");
    ui->scrollArea->setAlignment(Qt::AlignTop);

    //ApiLinkLabel Style
    ui->apiLinkLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
    ui->apiLinkLabel->setToolTip("Follow this link, create an account, and paste your API key at"
                                 " the right to unlock IP geolocation");
    set_stylesheet_from_json(*ui->apiLinkLabel, "apiLinkLabel", "Main");

    //Tabs Style
    ui->tabWidget->setCurrentIndex(0);

    //Save button Style
    ui->saveButton->disable();

    update_api_key_status();
    set_status_label_inactive();
}

void MainWindow::connect_buttons() {
    connect(ui->startButton, SIGNAL(clicked()), this, SLOT(start_button_clicked()));
    connect(ui->stopButton, SIGNAL(clicked()), this, SLOT(stop_button_clicked()));
    connect(ui->setApiKeyButton, SIGNAL(clicked()), this, SLOT(set_api_button_clicked()));
    connect(ui->clearButton, SIGNAL(clicked()), this, SLOT(remove_existing_packets()));
    connect(ui->saveButton, SIGNAL(clicked()), this, SLOT(save_to_file()));

    //Filter checkmarks
    connect(&ui->filterBox->model, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(refresh_packet_window()));
}

void MainWindow::set_status_label_active() {
    if(ui->statusLabel->movie()) {
        delete ui->statusLabel->movie();
    }

    QMovie *movie = new QMovie(MOVING_ICON);
    movie->setScaledSize(ui->statusLabel->size());
    ui->statusLabel->setMovie(movie);
    movie->start();
}
void MainWindow::set_status_label_inactive() {
    if(ui->statusLabel->movie()) {
        delete ui->statusLabel->movie();
    }

    QPixmap pixmap(STATIC_ICON);
    ui->statusLabel->setPixmap(pixmap.scaled(ui->statusLabel->size(), Qt::KeepAspectRatio));
    ui->statusLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    ui->statusLabel->show();
}

//ToDo: Dynamically instantiate different subclasses (in a single line/single function)?
//ToDo: This just seems messy in general, consider changing
//ToDo: Better way of handling clear_packets
void MainWindow::add_packet(const struct ip& ip_header, const int& packet_num) {
    if(MainWindow::clear_packets) {return;} //Necessary for now due to timing between threads

    Packet* packet = new Packet(ip_header, packet_num);
    packets.push_back(packet);
    display_packet(packet);
    QTextStream(stdout) << "Added Other Packet\n";
}
void MainWindow::add_packet(const struct ip& ip_header, const struct tcphdr& tcp_header, const int& packet_num) {
    if(MainWindow::clear_packets) {return;} //Necessary for now due to timing between threads

    TCPPacket* packet = new TCPPacket(ip_header, packet_num, tcp_header);
    packets.push_back(packet);

    if(ui->filterBox->tcp_filter_enabled()) {
        display_packet(packet);
    }

    QTextStream(stdout) << "Added TCP\n";
}
void MainWindow::add_packet(const struct ip& ip_header, const struct udphdr& udp_header, const int& packet_num) {
    if(MainWindow::clear_packets) {return;} //Necessary for now due to timing between threads

    UDPPacket* packet = new UDPPacket(ip_header, packet_num, udp_header);
    packets.push_back(packet);

    if(ui->filterBox->udp_filter_enabled()) {
        display_packet(packet);
    }

    QTextStream(stdout) << "Added UDP\n";
}
void MainWindow::add_packet(const struct ip& ip_header, const struct icmp& icmp_header, const int& packet_num) {
    if(MainWindow::clear_packets) {return;} //Necessary for now due to timing between threads

    ICMPPacket* packet = new ICMPPacket(ip_header, packet_num, icmp_header);
    packets.push_back(packet);

    if(ui->filterBox->icmp_filter_enabled()) {
        display_packet(packet);
    }

    display_packet(packet);
    QTextStream(stdout) << "Added ICMP\n";
}

void MainWindow::display_packet(Packet* packet) {
    InfoPane *infoPane = ui->infoPane;
    PacketLabel *label = new PacketLabel(packet, infoPane);

    label->setText(QString::fromStdString(packet->get_info()));
    ui->scrollArea->widget()->layout()->addWidget(label);

    add_line();
}

//ToDo: create a custom line class
void MainWindow::add_line() {
    QFrame *line = new QFrame();

    line->setObjectName(QString::fromUtf8("line"));
    line->setGeometry(QRect(320, 150, 118, 3));
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);

    ui->scrollArea->widget()->layout()->addWidget(line);
}

void MainWindow::closeEvent(QCloseEvent *ev) {
    Q_UNUSED(ev);
    closed = true;
}

void MainWindow::start_button_clicked() {
    ui->saveButton->disable();
    set_status_label_active();
    run_capture = true;
}
void MainWindow::stop_button_clicked() {
    set_status_label_inactive();
    run_capture = false;

    if(!packets.empty()) {
        ui->saveButton->enable();
    }
}

void MainWindow::clear_packet_display() {
    while(PacketLabel* label = ui->scrollArea->findChild<PacketLabel*>()) {
        delete label;
    }
    while(QFrame* line = ui->scrollArea->findChild<QFrame*>()) {
        delete line;
    }

    ui->infoPane->clear();
    ui->infoPane->set_style("Main");
}
void MainWindow::remove_existing_packets() {
    clear_packets = true;
    clear_packet_display();
    delete_packets();
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
            std::cout << "API Key file write error" << std::endl;
        }
    }

    update_api_key_status();
}

//ToDo: URGENT Handle api key(s) in a more sensible way
void MainWindow::update_api_key_status() {
    std::fstream api_key_file(API_KEY_FILE, std::fstream::in | std::fstream::app);
    std::string key;

    if(api_key_file) {
        std::getline(api_key_file, key);
        api_key_file.close();
    } else {
        std::cout << "API Key file read error\n";
    }

    if(key.length() > 0) {
        dummy_api_key = std::string(key.length(), '*');
        ui->apiKeyText->setText(QString::fromStdString(dummy_api_key));
        set_stylesheet_from_json(*ui->keyDetectedLabel, "keyDetectedLabel", "Main");
        ui->keyDetectedLabel->setText(" API key found!");
    } else {
        set_stylesheet_from_json(*ui->keyDetectedLabel, "keyDetectedLabel", "Alt");
        ui->keyDetectedLabel->setText(" No key detected");
    }
}

void MainWindow::refresh_packet_window() {
    clear_packet_display();
    add_valid_packets();
}

//ToDo: offload file saving to some util functions
void MainWindow::save_to_file() {
    std::time_t time_saved = std::time(nullptr);

    QString filename = QFileDialog::getExistingDirectory(this,
                                                        tr("Open Directory"),
                                                        "~",
                                                        QFileDialog::ShowDirsOnly
                                                        );

    std::string file_name_full = filename.toStdString();
    file_name_full += "/PacketRecord_";
    file_name_full += std::asctime(std::localtime(&time_saved));
    file_name_full += ".txt";
    std::replace(file_name_full.begin(), file_name_full.end(), ' ', '_');

    std::ofstream file(file_name_full);
    if(file) {
        for(auto& packet : packets) {
            file << packet->get_info() << "\n---------------\n";
        }

        file << std::endl;
        file.close();
        std::cout << "Save to file success\n";
    } else {
        std::cout << "Save to file error\n";
    }

    message_popup("Packet record saved as: " + file_name_full);
}


//ToDo: make this more efficient; loop through all available filter options
void MainWindow::add_valid_packets() {
    bool tcp_enabled = ui->filterBox->tcp_filter_enabled();
    bool udp_enabled = ui->filterBox->udp_filter_enabled();
    bool icmp_enabled = ui->filterBox->icmp_filter_enabled();

    for(auto& packet : packets) {
        if(tcp_enabled && dynamic_cast<TCPPacket*>(packet)) {
            display_packet(packet);
        } else if(udp_enabled && dynamic_cast<UDPPacket*>(packet)) {
            display_packet(packet);
        } else if(icmp_enabled && dynamic_cast<ICMPPacket*>(packet)) {
            display_packet(packet);
        }
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
