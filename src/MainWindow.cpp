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

    //Set scrollArea Style
    ui->scrollArea->setStyleSheet("background-color : white");
    ui->scrollArea->verticalScrollBar()->setStyleSheet("background-color : none");

    //Connect buttons
    connect(ui->startButton, SIGNAL(clicked()), this, SLOT(start_button_clicked()));
    connect(ui->stopButton, SIGNAL(clicked()), this, SLOT(stop_button_clicked()));

    //Misc style
    ui->statusLabel->setStyleSheet("background-color : red");
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::start_button_clicked() {
    ui->statusLabel->setStyleSheet("background-color : green");
    run_capture = true;
}
void MainWindow::stop_button_clicked() {
    ui->statusLabel->setStyleSheet("background-color : red");
    run_capture = false;
}

//ToDo: I swear there's a way to dynamically instantiate different subclasses (in a single line)
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
}

Ui::MainWindow* MainWindow::get_ui_pointer() {
    return this->ui;
}
