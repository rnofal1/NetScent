/* This file contains definitions for the MainWindow class (derived from QMainWindow)
 * Ramsey Nofal, 08/2023
 */


//Local
#include "MainWindow.h"


//Static variable definitions
Ui::MainWindow *MainWindow::ui = nullptr;


MainWindow::MainWindow(QWidget *parent)
                       : QMainWindow(parent),
                       run_capture(false) {
    ui = new Ui::MainWindow();
    ui->setupUi(this);

    //Set scrollArea Style; ToDo: put in separate function
    ui->scrollArea->setStyleSheet("background-color : white");
    ui->scrollArea->verticalScrollBar()->setStyleSheet("background-color : none");
    ui->scrollArea->horizontalScrollBar()->setStyleSheet("background-color : none");

    //Connect buttons; ToDo: put in separate function
    connect(ui->startButton, SIGNAL(clicked()), this, SLOT(start_button_clicked()));
    connect(ui->stopButton, SIGNAL(clicked()), this, SLOT(stop_button_clicked()));
    connect(ui->setApiKeyButton, SIGNAL(clicked()), this, SLOT(set_api_button_clicked()));

    //Misc style; ToDo: put in separate function
    ui->scrollArea->setAlignment(Qt::AlignTop);

    ui->apiLinkLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
    ui->apiLinkLabel->setToolTip("Follow this link, create an account, and paste your API key at"
                                 " the right to unlock IP geolocation");

    ui->tabWidget->setCurrentIndex(0);

    update_api_key_status();

    set_status_label_inactive();
}

MainWindow::~MainWindow() {
    delete ui;
}

Ui::MainWindow* MainWindow::get_ui_pointer() {
    return this->ui;
}

//ToDo: Dynamically instantiate different subclasses (in a single line)?
//ToDo: This just seems messy in general, consider changing
void MainWindow::add_packet(const struct ip& ip_header, const int& packet_num) {
    Packet* packet = new Packet(ip_header, packet_num);
    std::string info = packet->get_info();
    display_packet(info, packet);
    QTextStream(stdout) << "Added Other Packet\n" << QString::fromStdString(info);
}
void MainWindow::add_packet(const struct ip& ip_header, const struct tcphdr& tcp_header, const int& packet_num) {
    TCPPacket* packet = new TCPPacket(ip_header, packet_num, tcp_header);
    std::string info = packet->get_info();
    display_packet(info, packet);
    QTextStream(stdout) << "Added TCP\n" << QString::fromStdString(info);
}
void MainWindow::add_packet(const struct ip& ip_header, const struct udphdr& udp_header, const int& packet_num) {
    UDPPacket* packet = new UDPPacket(ip_header, packet_num, udp_header);
    std::string info = packet->get_info();
    display_packet(info, packet);
    QTextStream(stdout) << "Added UDP\n" << QString::fromStdString(info);
}
void MainWindow::add_packet(const struct ip& ip_header, const struct icmp& icmp_header, const int& packet_num) {
    ICMPPacket* packet = new ICMPPacket(ip_header, packet_num, icmp_header);
    std::string info = packet->get_info();
    display_packet(info, packet);
    QTextStream(stdout) << "Added ICMP\n" << QString::fromStdString(info);
}

void MainWindow::display_packet(const std::string& packetText, Packet* packet) {
    //Add Label
    QTextBrowser *infoPane = ui->textBrowser;
    CustomLabel *label = new CustomLabel(packet, infoPane);

    label->setText(QString::fromStdString(packetText));
    label->setMinimumHeight(50);
    label->setMaximumHeight(50);

    ui->scrollArea->widget()->layout()->addWidget(label);

    //Add Line
    //ToDo: turn this into its own class
    QFrame *line = new QFrame();

    line->setObjectName(QString::fromUtf8("line"));
    line->setGeometry(QRect(320, 150, 118, 3));
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);

    ui->scrollArea->widget()->layout()->addWidget(line);
}

//ToDo: URGENT Handle api key(s) in a more sensible way
void MainWindow::update_api_key_status() {
    if(std::filesystem::exists(API_KEY_FILE)) {
        ui->keyDetectedLabel->setStyleSheet("background-color : green");
        ui->keyDetectedLabel->setText(" API key found!");
    } else {
        ui->keyDetectedLabel->setStyleSheet("background-color : orangered");
        ui->keyDetectedLabel->setText(" No key detected");
    }
}

void MainWindow::set_status_label_active() {
    QMovie *movie = new QMovie("icons/icon_rotate_cube.gif");
    movie->setScaledSize(ui->statusLabel->size());
    ui->statusLabel->setMovie(movie);
    movie->start();
}
void MainWindow::set_status_label_inactive() {
    int w = ui->statusLabel->width();
    int h = ui->statusLabel->height();

    QPixmap pixmap("icons/icon.jpg");
    ui->statusLabel->setPixmap(pixmap.scaled(w, h, Qt::KeepAspectRatio));
    ui->statusLabel->setMask(pixmap.mask());
    ui->statusLabel->show();
}

void MainWindow::start_button_clicked() {
    //ui->statusLabel->setStyleSheet("background-color : green");
    set_status_label_active();
    run_capture = true;
}
void MainWindow::stop_button_clicked() {
    //ui->statusLabel->setStyleSheet("background-color : red");
    set_status_label_inactive();
    run_capture = false;
}

//ToDo: URGENT Handle api key(s) in a more sensible way
void MainWindow::set_api_button_clicked() {
    std::string input_key = ui->apiKeyText->text().toStdString();

    if(!input_key.empty()) {
        std::fstream file(API_KEY_FILE, std::fstream::in | std::fstream::out | std::fstream::trunc);
        file << ui->apiKeyText->text().toStdString() << std::endl;
        file.close();
        ui->apiKeyText->setText("");
    }

    update_api_key_status();
}
