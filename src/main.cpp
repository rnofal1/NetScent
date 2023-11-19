/* This file contains the top-level main function to initialize and
 * run this program
 *
 * Ramsey Nofal, 08/2023
 */


//Standard Qt
#include <QtConcurrent>
#include <QScopedPointer>

//Local
#include "PacketCap.h"
#include "CustomApplication.h"
#include "SharedQueue.h"
#include "util.h"

//Defines
#define PACKET_QUEUE_CAPACITY 9999


int main(int argc, char *argv[]) {
    QScopedPointer<CustomApplication> app(new CustomApplication(argc, argv));

    SharedQueue<Packet*> packet_queue(PACKET_QUEUE_CAPACITY);

    MainWindow *main_window = new MainWindow(&packet_queue);
    main_window->show();

    PacketCap packet_capturer(main_window, &packet_queue);
    QFuture<int> packet_cap_thread = QtConcurrent::run(&PacketCap::run_packet_cap, &packet_capturer);

    int app_success = !app->exec();
    int packet_cap_success = !thread_handler(packet_cap_thread);
    qDebug() << "Outside" << app_success << packet_cap_success;
    return !(app_success && packet_cap_success);
}
