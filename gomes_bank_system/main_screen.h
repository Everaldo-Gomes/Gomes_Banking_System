#ifndef MAIN_SCREEN_H
#define MAIN_SCREEN_H

#include <QWidget>
#include <QMenuBar>
#include <QComboBox>
#include <QMessageBox>
#include <QDebug>

#include <global_variables.h>
#include "login.h"
#include "search_staff.h"
#include "search_customer_account.h"
#include "search_loan.h"
#include "add_staff.h"
#include "block_staff.h"
#include "unlock_staff.h"
#include "change_staff_info.h"
#include "add_customer_account.h"
#include "block_customer_account.h"
#include "unlock_customer_account.h"
#include "change_customer_info.h"
#include "check_balance.h"
#include "deposit.h"
#include "withdrawal.h"
#include "transfer.h"
#include "loan.h"
#include "pay_loan.h"
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

    void on_search_customer_account_button_clicked();

    void on_check_balance_button_clicked();

    void on_deposit_button_clicked();

    void on_withdrawal_button_clicked();

    void on_transfer_button_clicked();

    void on_loan_button_clicked();

    void on_pay_loan_button_clicked();

    void on_search_loan_button_clicked();

private:
    Ui::main_screen *ui;
};

#endif // MAIN_SCREEN_H
