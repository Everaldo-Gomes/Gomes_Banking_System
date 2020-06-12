#ifndef MAIN_SCREEN_H
#define MAIN_SCREEN_H

#include <QWidget>
#include <QMenuBar>
#include <QDebug>
#include <global_variables.h>
#include "login.h"

namespace Ui {
class main_screen;
}

class main_screen : public QWidget
{
    Q_OBJECT

public:
    explicit main_screen(QWidget *parent = nullptr);
    ~main_screen();


private slots:
    void on_logoutbutton_clicked();

    void on_search_staff_button_clicked();

private:
    Ui::main_screen *ui;

};

#endif // MAIN_SCREEN_H
