/* Classic main file; contains the top-level main function to initialize and
 * run this program
 *
 * Ramsey Nofal, 08/2023
 */


//Standard Qt
#include <QApplication>

//Local
#include "PacketCap.h"


int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon("icons/icon.jpg"));

    MainWindow w;
    w.show();

    PacketCap packet_capturer(&w);
    std::thread t1(&PacketCap::run_packet_cap, packet_capturer);

    int success = a.exec();

    t1.join();

    return success;
}
