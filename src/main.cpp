#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    PacketCap packet_capturer;
    std::thread t1(&PacketCap::run_packet_cap, packet_capturer);

    t1.detach();

    return a.exec();
}
