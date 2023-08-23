/* This file contains declarations for the InfoPane class (derived from QTextBrowser)
 * Ramsey Nofal, 08/2023
 */


#ifndef INFOPANE_H
#define INFOPANE_H


//Standard Qt
#include <QTextBrowser>


class InfoPane : public QTextBrowser {
    Q_OBJECT
public:
    InfoPane(QWidget *parent = nullptr);

    void add_style(const std::string& style_name, const std::string& style_val);
    void set_style(const std::string& style_name);

private:
    std::map<std::string, std::string> style_map;
};

#endif // INFOPANE_H
