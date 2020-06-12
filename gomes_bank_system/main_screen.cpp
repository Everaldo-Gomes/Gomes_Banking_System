#include "main_screen.h"
#include "ui_main_screen.h"

#include "login.h"
#include "search_staff.h"
#include "add_staff.h"

main_screen::main_screen(QWidget *parent) : QWidget(parent), ui(new Ui::main_screen) {
    ui->setupUi(this);

    //this->showMaximized();  //show screen maximazed
    //setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);
    //tabs names
    ui->tabWidget->setTabText(0, "GBS");
    ui->tabWidget->setTabText(1, "Search");
    ui->tabWidget->setTabText(2, "Staff");
    ui->tabWidget->setTabText(3, "Customer - Account");
    ui->tabWidget->setTabText(4, "Money");
    ui->tabWidget->setTabText(5, "Help");

    //info about the connected staff
    ui->connected_staff_id_value->setText(connected_id);
    ui->connected_staff_name_value->setText(connected_staff);
    ui->connected_staff_sector_value->setText(connected_sector);
}

main_screen::~main_screen() {
    delete ui;
}

//logout button
void main_screen::on_logoutbutton_clicked() {
    this->close();
    Login *l = new Login();
    l->show(); //show login screen
}

//open search staff window
void main_screen::on_search_staff_button_clicked() {
    search_staff *sf = new search_staff();
    sf->show();
}













