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
#define PACKET_QUEUE_CAPACITY 9999


int main(int argc, char *argv[]) {
    CustomApplication app(argc, argv);

    SharedQueue<Packet*> packet_queue(PACKET_QUEUE_CAPACITY);

    MainWindow main_window(&packet_queue);
    main_window.show();

    PacketCap packet_capturer(&main_window, &packet_queue);
    QFuture<void> packet_cap_thread = QtConcurrent::run([&packet_capturer]
                                                        {packet_capturer.run_packet_cap();
                                                    });

    return app.exec();
}
