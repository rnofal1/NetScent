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

    std::ifstream file(STYLE_FILE);
    if(file) {
        nlohmann::json style_json = nlohmann::json::parse(file);
        a.setWindowIcon(QIcon(QString::fromStdString(std::string(style_json["Misc"]["icon-path"]))));
        file.close();
    } else {
        std::cout << "Style file not found; Dynamic style elements will not work\n";
    }

    MainWindow w;
    w.show();

    PacketCap packet_capturer(&w);
    std::thread t1(&PacketCap::run_packet_cap, packet_capturer);

    int success = a.exec();

    t1.join();

    return success;
}
