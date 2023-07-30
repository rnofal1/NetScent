#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <stdlib.h>     //for using the function sleep
#include <QScrollBar>
#include "customlabel.h"

int PacketCap::num_packets = 0;
int PacketCap::link_header_len = 0;
pcap_t* PacketCap::handle = nullptr;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //Set scrollArea Style
    ui->scrollArea->setStyleSheet("background-color : white");
    ui->scrollArea->verticalScrollBar()->setStyleSheet("background-color : none");

    //Label stuff (move)
    QTextBrowser *infoPane = ui->textBrowser;
    for(int i = 0; i < 55; ++i) {
        CustomLabel *label = new CustomLabel(infoPane);
        label->setText(QString::fromStdString("hi" + std::to_string(i)));
        label->setMinimumHeight(50);
        ui->scrollArea->widget()->layout()->addWidget(label);
    }

    //Connect buttons
    connect(ui->startButton, SIGNAL(clicked()), this, SLOT(start_button_clicked()));


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::start_button_clicked() {
    ui->scrollArea->setStyleSheet("background-color : black");
}
void MainWindow::stop_button_clicked() {
    ui->scrollArea->setStyleSheet("background-color : black");
}
