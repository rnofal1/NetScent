/* This file contains the top-level main function to initialize and
 * run this program
 *
 * Ramsey Nofal, 08/2023
 */


/* Standard Qt */
#include <QtConcurrent>

/* Local */
#include "PacketCap.h"
#include "CustomApplication.h"
#include "SharedQueue.h"
#include "util.h"

/* Defines */
#define PACKET_QUEUE_CAPACITY 9999


int main(int argc, char *argv[]) {
    //Initilize the manager for the main Qt event loop and control flow
    QScopedPointer<CustomApplication> app(new CustomApplication(argc, argv));

    //Create shared storage for intercepted network packets
    SharedQueue<Packet*> packet_queue(PACKET_QUEUE_CAPACITY);

    //Initialize the main framework for UI objects
    QPointer<MainWindow> main_window(new MainWindow(&packet_queue));
    main_window->show();

    //Initialize and run the process to capture network packets
    PacketCap packet_capturer(main_window, &packet_queue);
    QFuture<int> packet_cap_thread = QtConcurrent::run(&PacketCap::run_packet_cap, &packet_capturer);

    //Run the Qt event loop
    int app_success = !app->exec();

    //Wait for other thread(s) to complete
    int packet_cap_success = !thread_handler(packet_cap_thread);

    return !(app_success && packet_cap_success);
}
