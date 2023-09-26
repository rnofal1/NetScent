/* This file contains declarations for the CustomApplication class
 * (derived from QApplication)
 * Ramsey Nofal, 08/2023
 */


#ifndef CUSTOMAPPLICATION_H
#define CUSTOMAPPLICATION_H


//Standard Qt
#include <QApplication>


class CustomApplication : public QApplication {
    Q_OBJECT
public:
    CustomApplication(int &argc, char **argv);

    void add_style(const std::string& style_name, const std::string& style_val);
    void set_style(const std::string& style_name);

private:
    std::map<std::string, std::string> style_map;
};

#endif // CUSTOMAPPLICATION_H
