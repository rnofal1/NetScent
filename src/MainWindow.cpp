/* This file contains definitions for the MainWindow class (derived from QMainWindow)
 * Ramsey Nofal, 08/2023
 */


//Local
#include "MainWindow.h"


//Static variable definitions
Ui::MainWindow *MainWindow::ui = nullptr;


MainWindow::MainWindow(QWidget *parent)
                       : QMainWindow(parent),
                       run_capture(false),
                       closed(false) {
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
    delete ui;
}

Ui::MainWindow* MainWindow::get_ui_pointer() {
    return this->ui;
}

//ToDo: store style preferences in a file (json, xml) or modify the ui xml directly
void MainWindow::set_widgets_style() {
    //ScrollArea Style
    ui->scrollArea->setStyleSheet("background-color : rgb(66, 69, 73)");
    ui->scrollArea->verticalScrollBar()->setStyleSheet("background-color : none; color : black");
    ui->scrollArea->horizontalScrollBar()->setStyleSheet("background-color : none; color : black");
    ui->scrollArea->setAlignment(Qt::AlignTop);

    //ApiLinkLabel Style
    ui->apiLinkLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
    ui->apiLinkLabel->setToolTip("Follow this link, create an account, and paste your API key at"
                                 " the right to unlock IP geolocation");
    ui->apiLinkLabel->setStyleSheet("QToolTip { background-color: white; }");

    //Api key detected Style
    ui->keyDetectedLabel->setStyleSheet("color : black");

    //Tabs Style
    ui->tabWidget->setCurrentIndex(0);

    update_api_key_status();
    set_status_label_inactive();
}

void MainWindow::connect_buttons() {
    connect(ui->startButton, SIGNAL(clicked()), this, SLOT(start_button_clicked()));
    connect(ui->stopButton, SIGNAL(clicked()), this, SLOT(stop_button_clicked()));

    connect(ui->setApiKeyButton, SIGNAL(clicked()), this, SLOT(set_api_button_clicked()));
}

//ToDo: URGENT Handle api key(s) in a more sensible way
void MainWindow::update_api_key_status() {
    std::ifstream api_key_file(API_KEY_FILE);
    std::string key;

    if(api_key_file) {
        std::getline(api_key_file, key);
        api_key_file.close();
    }

    if(key.length() > 0) {
        dummy_api_key = std::string(key.length(), '*');
        ui->apiKeyText->setText(QString::fromStdString(dummy_api_key));
        ui->keyDetectedLabel->setStyleSheet("background-color : green; color : black");
        ui->keyDetectedLabel->setText(" API key found!");
    } else {
        ui->keyDetectedLabel->setStyleSheet("background-color : orangered; color : black");
        ui->keyDetectedLabel->setText(" No key detected");
    }
}

void MainWindow::set_status_label_active() {
    if(ui->statusLabel->movie()) {
        delete ui->statusLabel->movie();
    }

    QMovie *movie = new QMovie("icons/icon_rotate_cube.gif");
    movie->setScaledSize(ui->statusLabel->size());
    ui->statusLabel->setMovie(movie);
    movie->start();
}
void MainWindow::set_status_label_inactive() {
    if(ui->statusLabel->movie()) {
        delete ui->statusLabel->movie();
    }

    QPixmap pixmap("icons/icon.png");
    ui->statusLabel->setPixmap(pixmap.scaled(ui->statusLabel->size(), Qt::KeepAspectRatio));
    ui->statusLabel->show();
}

//ToDo: Dynamically instantiate different subclasses (in a single line/single function)?
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
    QTextBrowser *infoPane = ui->textBrowser;
    CustomLabel *label = new CustomLabel(packet, infoPane);

    label->setText(QString::fromStdString(packetText));
    label->setMinimumHeight(50);
    label->setMaximumHeight(50);

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
    set_status_label_active();
    run_capture = true;
}
void MainWindow::stop_button_clicked() {
    set_status_label_inactive();
    run_capture = false;
}

//ToDo: URGENT Handle api key(s) in a more sensible way
void MainWindow::set_api_button_clicked() {
    std::string input_key = ui->apiKeyText->text().toStdString();

    if(input_key != dummy_api_key) {
        std::fstream file(API_KEY_FILE, std::fstream::in | std::fstream::out | std::fstream::trunc);
        file << input_key << std::endl;
        file.close();
    }

    update_api_key_status();
}
