#include "search_customer_account.h"
#include "ui_search_customer_account.h"

QString found_cus_acc_id;

search_customer_account::search_customer_account(QWidget *parent) : QWidget(parent), ui(new Ui::search_customer_account) {
    ui->setupUi(this);
    ui->cpf_field_input->setValidator(new QDoubleValidator(0, 999999999999, 2, this)); //allow only numbers. need to be double becuase the range is too long
    ui->error_message->setStyleSheet("QLabel{ color: red;}"); //error message color
    ui->cpf_field_input->setFocus();
}

search_customer_account::~search_customer_account() {
    delete ui;
}

//close button
void search_customer_account::on_close_button_clicked() {
    this->close();
}

//clean all information when change cpf
void search_customer_account::on_cpf_field_input_textChanged() {
    ui->error_message->setText("");
    ui->name_output->setText("");
    ui->cpf_output->setText("");
    ui->address_output ->setText("");
    ui->phone_output->setText("");
    ui->birthday_output->setText("");
    ui->account_output->setText("");
    ui->email_output->setText("");
    ui->created_output->setText("");
    ui->id_output->setText("");
    ui->status_output->setText("");
    ui->blocked_times_output->setText("");
}

//search button
void search_customer_account::on_search_button_clicked() {
    QString typed_cpf = ui->cpf_field_input->text();

    if(typed_cpf == "") { ui->error_message->setText("Enter a valid CPF."); }
    else if(connect_database()) {
        if(!search_customer_cpf(typed_cpf)) {

            //check if the staff is blocked
            bool is_blocked = customer_blocked(typed_cpf);
            if(is_blocked) {
                ui->status_output->setText("Blocked");
                ui->status_output->setStyleSheet("QLabel{ color: red;}");
            }
            else {
                ui->status_output->setText("Unlocked");
                ui->status_output->setStyleSheet("QLabel{ color: green;}");
            }

            //show information
            QSqlQuery get_info;
            get_info.exec("SELECT * FROM customer");

            while(get_info.next()) {
                        found_cus_acc_id = get_info.value(0).toString();
                QString found_name       = get_info.value(1).toString();
                QString found_cpf        = get_info.value(2).toString();
                QString found_address    = get_info.value(3).toString();
                QString found_phone      = get_info.value(4).toString();
                QString found_birth      = get_info.value(5).toString();
                QString found_email      = get_info.value(6).toString();
                QString found_created_in = get_info.value(7).toString();

                if(typed_cpf == found_cpf) {
                    ui->name_output->setText(found_name);
                    ui->cpf_output->setText(found_cpf);
                    ui->address_output ->setText(found_address);
                    ui->phone_output->setText(found_phone);
                    ui->birthday_output->setText(found_birth);
                    ui->email_output->setText(found_email);
                    ui->created_output->setText(found_created_in);
                    ui->account_output->setText(get_acc_number(typed_cpf));
                    ui->id_output->setText(found_cus_acc_id);
                    break;
                }
            }
            ui->account_output->setText(get_acc_number(typed_cpf)); //show account number

            //customer info
            QSqlQuery count_blocked_times;
            count_blocked_times.prepare("SELECT times FROM many_times_customer_account_blocked WHERE customer_id = ?");
            count_blocked_times.addBindValue(found_cus_acc_id.toInt());
            count_blocked_times.exec();

            while(count_blocked_times.next()) {
                QString times = count_blocked_times.value(0).toString();
                ui->blocked_times_output->setText(times);
            }

            //if the staff wasn't blocked yet put 0
            if(ui->blocked_times_output->text() == "") { ui->blocked_times_output->setText("0"); }
        }
        else { ui->error_message->setText("CPF is not registerd"); }
    }
    else { ui->error_message->setText("You re not connected"); }
}


