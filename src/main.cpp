/* This file contains the top-level main function to initialize and
 * run this program
 *
 * Ramsey Nofal, 08/2023
 */


//Standard Qt
#include <QtConcurrent>

//Local
#include "PacketCap.h"
#include "CustomApplication.h"
#include "SharedQueue.h"

//Defines
#define PACKET_QUEUE_CAPACITY 10

int main(int argc, char *argv[]) {
    CustomApplication a(argc, argv);

    SharedQueue<Packet*> packet_queue(PACKET_QUEUE_CAPACITY);

    MainWindow w(&packet_queue);
    w.show();

    PacketCap packet_capturer(&w, &packet_queue);
    QFuture<void> future = QtConcurrent::run([&packet_capturer] {packet_capturer.run_packet_cap();});

    return a.exec();
}
