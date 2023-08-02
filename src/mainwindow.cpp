#include "mainwindow.h"
#include <stdlib.h>     //for using the function sleep

Ui::MainWindow *MainWindow::ui = nullptr;
std::unordered_map<int, Packet*> MainWindow::packet_map;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), run_capture(false)
{
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

MainWindow::~MainWindow()
{
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
//ToDo: WARNING/ALERT change using a void pointer to something a bit more sensible
void MainWindow::addPacket(const struct ip& ip_header, const int& packet_num) {
    packet_map[packet_num] = new Packet(ip_header);
    std::string info = packet_map[packet_num]->get_info();
    displayPacket(info);
    QTextStream(stdout) << "Added Other\n" << QString::fromStdString(info);
}
void MainWindow::addPacket(const struct ip& ip_header, const struct tcphdr& tcp_header, const int& packet_num) {
    packet_map[packet_num] = new TCPPacket(ip_header, tcp_header);
    std::string info = packet_map[packet_num]->get_info();
    displayPacket(info);
    QTextStream(stdout) << "Added TCP\n" << QString::fromStdString(info);
}
void MainWindow::addPacket(const struct ip& ip_header, const struct udphdr& udp_header, const int& packet_num) {
    packet_map[packet_num] = new UDPPacket(ip_header, udp_header);
    std::string info = packet_map[packet_num]->get_info();
    displayPacket(info);
    QTextStream(stdout) << "Added UDP\n" << QString::fromStdString(info);
}
void MainWindow::addPacket(const struct ip& ip_header, const struct icmp& icmp_header, const int& packet_num) {
    packet_map[packet_num] = new ICMPPacket(ip_header, icmp_header);
    std::string info = packet_map[packet_num]->get_info();
    displayPacket(info);
    QTextStream(stdout) << "Added ICMP\n" << QString::fromStdString(info);
}

void MainWindow::displayPacket(const std::string& packetText) {
    QTextBrowser *infoPane = ui->textBrowser;
    CustomLabel *label = new CustomLabel(infoPane);
    label->setText(QString::fromStdString(packetText));
    label->setMinimumHeight(50);
    label->setMaximumHeight(50);
    ui->scrollArea->widget()->layout()->addWidget(label);
}

Ui::MainWindow* MainWindow::getUiPointer() {
    return this->ui;
}
