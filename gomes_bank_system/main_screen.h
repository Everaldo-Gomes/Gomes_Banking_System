#ifndef MAIN_SCREEN_H
#define MAIN_SCREEN_H

#include <QWidget>
#include <QMenuBar>
#include <QDebug>

#include <global_variables.h>
#include "login.h"
#include "search_staff.h"
#include "search_customer.h"
#include "search_account.h"
#include "add_staff.h"
#include "block_staff.h"
#include "unlock_staff.h"
#include "change_staff_info.h"
#include "add_customer_account.h"
#include "support.h"
#include "about.h"

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

    void on_search_customer_button_clicked();

    void on_search_account_button_clicked();

    void on_about_button_clicked();

    void on_support_button_clicked();

    void on_add_staff_button_clicked();

    void on_block_staff_button_clicked();

    void on_unlock_staff_button_clicked();

    void on_change_info_staff_button_clicked();

    void on_add_customer_account_button_clicked();

    void on_block_customer_account_button_clicked();

    void on_unlock_customer_account_button_clicked();

    void on_change_info_customer_account_button_clicked();

private:
    Ui::main_screen *ui;
};

#endif // MAIN_SCREEN_H
