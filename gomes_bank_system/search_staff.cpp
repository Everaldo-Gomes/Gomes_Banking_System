#include "search_staff.h"
#include "ui_search_staff.h"

search_staff::search_staff(QWidget *parent) : QWidget(parent), ui(new Ui::search_staff) {
    ui->setupUi(this);
    ui->cpf_field_input->setValidator(new QDoubleValidator(0, 999999999999, 2, this)); //allow only numbers. need to be double becuase the range is too long
}

search_staff::~search_staff() {
    delete ui;
}

//close window button
void search_staff::on_close_button_clicked() {
    this->close();
}

//search again button
void search_staff::on_search_again_button_clicked() {
    //clean all fields
    ui->cpf_field_input->setText("");
    ui->cpf_field_input->setFocus();
    ui->name_output->setText("");
    ui->cpf_output->setText("");
    ui->address_output->setText("");
    ui->phone_output->setText("");
    ui->email_output->setText("");
    ui->sector_output->setText("");
    ui->employee_since_output->setText("");
    ui->blocked_times_output->setText("");
}


