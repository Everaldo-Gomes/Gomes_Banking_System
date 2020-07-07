#include "check_balance.h"
#include "ui_check_balance.h"

check_balance::check_balance(QWidget *parent) : QWidget(parent), ui(new Ui::check_balance) {
    ui->setupUi(this);
    ui->cpf_field_input->setValidator(new QDoubleValidator(0, 999999999999, 2, this)); //allow only numbers. need to be double becuase the range is too long
    ui->error_message->setStyleSheet("QLabel{ color: red;}"); //error message color
    ui->cpf_field_input->setFocus();
}

check_balance::~check_balance() { delete ui; }

//close button
void check_balance::on_close_button_clicked() { this->close(); }

//clean of information
void check_balance::on_cpf_field_input_textChanged() {
    ui->error_message->setText("");
    ui->name_output->setText("");
    ui->cpf_output->setText("");
    ui->account_output->setText("");
    ui->amount_output->setText("");
}

//search button
void check_balance::on_search_button_clicked() {
    QString typed_cpf = ui->cpf_field_input->text();

    if(typed_cpf == "") { ui->error_message->setText("Enter a valid CPF"); }
    else if(connect_database()) {
        if(!search_customer_cpf(typed_cpf)) {

            //if is blocked dont show anything
            if(customer_blocked(typed_cpf)) { ui->error_message->setText("Customer blocked"); }
            else {
                //show information
                QSqlQuery get_info;
                get_info.exec("SELECT * FROM customer");

                while(get_info.next()) {
                    QString found_name = get_info.value(1).toString();
                    QString found_cpf  = get_info.value(2).toString();

                    if(typed_cpf == found_cpf) {
                        ui->name_output->setText(found_name);
                        ui->cpf_output->setText(found_cpf);
                        ui->account_output->setText(get_acc_number(typed_cpf));

                        //verify if the amount has decimal points
                        QString found_amount = get_account_amount(search_customer_id_by_cpf(typed_cpf));

                        if(!has_decimal_point(found_amount)) { ui->amount_output->setText("R$ " + found_amount + ".00"); }
                        else { ui->amount_output->setText("R$ " + found_amount); }

                        break;
                    }
                }
                ui->account_output->setText(get_acc_number(typed_cpf)); //show account number
            }
        }
        else { ui->error_message->setText("CPF is not registerd"); }
    }
    else { ui->error_message->setText("You re not connected"); }
}


