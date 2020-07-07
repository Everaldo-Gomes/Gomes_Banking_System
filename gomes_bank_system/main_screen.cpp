#include "main_screen.h"
#include "ui_main_screen.h"

main_screen::main_screen(QWidget *parent) : QWidget(parent), ui(new Ui::main_screen) {
    ui->setupUi(this);
    this->setWindowFlags(Qt::WindowMinimizeButtonHint); //only show minimize button

    //tabs names
    ui->tabWidget->setTabText(0, "GBS");
    ui->tabWidget->setTabText(1, "Search");
    ui->tabWidget->setTabText(2, "Staff");
    ui->tabWidget->setTabText(3, "Money");
    ui->tabWidget->setTabText(4, "Customer - Account");
    ui->tabWidget->setTabText(5, "Pay");
    ui->tabWidget->setTabText(6, "Help");

    //info about the connected staff
    ui->connected_staff_id_value->setText(connected_id);
    ui->connected_staff_name_value->setText(connected_staff);
    ui->connected_staff_sector_value->setText(connected_sector);
}

main_screen::~main_screen() { delete ui; }

//logout button
void main_screen::on_logoutbutton_clicked() {

    //ask for a confirmation
    QMessageBox::StandardButton confirmation;
    confirmation = QMessageBox::question(this, "Confirm", "Are you sure?", QMessageBox::Yes | QMessageBox::No);

    if(confirmation == QMessageBox::No) { /*do nothing*/ }
    else {
        this->close();
        Login *l = new Login();
        l->show(); //show login screen
    }
}

//open search staff window
void main_screen::on_search_staff_button_clicked() {
    search_staff *sf = new search_staff();
    sf->show();
}

//open search customer/account window
void main_screen::on_search_customer_account_button_clicked() {
    search_customer_account *sca = new search_customer_account;
    sca->show();
}

//search loan button
void main_screen::on_search_loan_button_clicked() {
    search_loan *sl = new search_loan;
    sl->show();
}

//open add staff window
void main_screen::on_add_staff_button_clicked() {
    add_staff *af = new add_staff();
    af->show();
}

//open block staff window
void main_screen::on_block_staff_button_clicked() {
    block_staff *bs = new block_staff();
    bs->show();
}

//open unlock staff window
void main_screen::on_unlock_staff_button_clicked() {
    unlock_staff *us = new unlock_staff();
    us->show();
}

//open change staff info window
void main_screen::on_change_info_staff_button_clicked() {
    change_staff_info *cf = new change_staff_info();
    cf->show();
}

//open add customer/account window
void main_screen::on_add_customer_account_button_clicked() {
    add_customer_account *aca = new add_customer_account;
    aca->show();
}

//open block customer/account window
void main_screen::on_block_customer_account_button_clicked() {
    block_customer_account *bca = new block_customer_account;
    bca->show();
}

//open unlock customer/account window
void main_screen::on_unlock_customer_account_button_clicked() {
    unlock_customer_account *uca = new unlock_customer_account;
    uca->show();
}

//open change info customer/account window
void main_screen::on_change_info_customer_account_button_clicked() {
    change_customer_info *cci = new change_customer_info;
    cci->show();
}

//check balance option
void main_screen::on_check_balance_button_clicked() {
    check_balance *cb = new check_balance;
    cb->show();
}

//deposit option
void main_screen::on_deposit_button_clicked() {
    deposit *d = new deposit;
    d->show();
}

//withdrawal option
void main_screen::on_withdrawal_button_clicked() {
    withdrawal *w = new withdrawal;
    w->show();
}

//transfer option
void main_screen::on_transfer_button_clicked() {
    transfer *t = new transfer;
    t->show();
}

//loan option
void main_screen::on_loan_button_clicked() {
    loan *l = new loan;
    l->show();
}

//pay loan button
void main_screen::on_pay_loan_button_clicked() {
    pay_loan *pl = new pay_loan;
    pl->show();
}

//open about window
void main_screen::on_about_button_clicked() {
    about *ab = new about();
    ab->show();
}

//open support window
void main_screen::on_support_button_clicked() {
    support *s = new support();
    s->show();
}
