/* This file contains declarations for the CustomButton class (derived from QPushButton)
 * Ramsey Nofal, 08/2023
 */


#ifndef CUSTOMBUTTON_H
#define CUSTOMBUTTON_H

//Standard Qt
#include <QPushButton>


/* The CustomButton class is derived from the QPushButton class, with added stylistic
 * functionality
 */
class CustomButton : public QPushButton {
    Q_OBJECT
public:
    //using QPushButton::QPushButton;
    CustomButton(QWidget *parent = nullptr);

    void enable();
    void disable();

    void add_style(const std::string& style_name, const std::string& style_val);
    void set_style(const std::string& style_name);

protected:
    void enterEvent(QEnterEvent *ev) override;
    void leaveEvent(QEvent *ev) override;

private:
    std::map<std::string, std::string> style_map;
};

#endif // CUSTOMBUTTON_H
