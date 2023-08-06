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

    MainWindow w;
    w.show();

    PacketCap packet_capturer(&w);
    std::thread t1(&PacketCap::run_packet_cap, packet_capturer);
    t1.detach(); //ToDo: this threading could be handled better

    return a.exec();
}
