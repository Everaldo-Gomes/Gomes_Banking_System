#include "add_customer_account.h"
#include "ui_add_customer_account.h"

add_customer_account::add_customer_account(QWidget *parent) : QWidget(parent), ui(new Ui::add_customer_account) {
    ui->setupUi(this);
    ui->cpf_input->setValidator(new QDoubleValidator(0, 999999999999, 2, this)); //allow only numbers. need to be double becuase the range is too long
    ui->phone_input->setValidator(new QDoubleValidator(0, 999999999999, 2, this));
    ui->amount_input->setValidator(new QDoubleValidator(0, 999999999999, 2, this));
    ui->error_message->setStyleSheet("QLabel{ color: red;}"); //error message color
    ui->name_input->setFocus();

    //date box
    ui->day_input->setRange(1,31);
    ui->month_input->setRange(1,12);
    ui->year_input->setRange(1950,2020);
}

add_customer_account::~add_customer_account() {
    delete ui;
}

//close window
void add_customer_account::on_cancel_button_clicked() {
    this->close();
}

//add button
void add_customer_account::on_add_button_clicked() {
    QString typed_name             = ui->name_input->text();
    QString typed_cpf              = ui->cpf_input->text();
    QString typed_address          = ui->address_input->text();
    QString typed_phone            = ui->phone_input->text();
    QString typed_day              = ui->day_input->text();
    QString typed_month            = ui->month_input->text();
    QString typed_year             = ui->year_input->text();
    QString typed_email            = ui->email_input->text();
    QString typed_password         = ui->account_password_input->text();
    QString typed_confirm_password = ui->account_confirm_passowrd_input->text();
    QString full_data = typed_year + "-" + typed_month + "-" + typed_day;
    QString typed_amount           = ui->amount_input->text();
    double amount                  = typed_amount.toDouble();

    //verify if all field were filled
    if(typed_name == "" || typed_cpf == "" || typed_address == "" || typed_phone == "" || typed_amount == "" ||
      typed_email == "" || typed_password == "" || typed_confirm_password == "") {
        ui->error_message->setText("Enter all needed information");
    }

    //verify if the amount given is the minimum accepted
    else if(amount < 50.00) {
        ui->error_message->setText("The minimum value is R$50.00");
    }

    //verify if the typed passowords match
    else if(typed_password != typed_confirm_password) { ui->error_message->setText("Passwords don't match"); }

    //ask for a confimation before adding
    else {
        QMessageBox::StandardButton confirmation;
        confirmation = QMessageBox::question(this, "Confirm", "Are you sure?",
                                      QMessageBox::Yes | QMessageBox::No);

        if(confirmation == QMessageBox::No) { /*do nothing*/ }

        //do not make a single "if" because it has two types of errors
        else if(connect_database()) {
             if(search_customer_cpf(typed_cpf)) {  //verify if the staff already exist

                 //encrypt password
                 Login *ll = new Login();
                 QString encrypted_password = ll->encrypt_password(typed_password);

                 //add customer
                 QSqlQuery add_customer;
                 add_customer.prepare("INSERT INTO customer (full_name, cpf, address, phone_number, birthday, email, created_in, responsible_staff_id)"
                                     "VALUES (?,?,?,?,?,?,?,?)");
                 add_customer.addBindValue(typed_name);
                 add_customer.addBindValue(typed_cpf);
                 add_customer.addBindValue(typed_address);
                 add_customer.addBindValue(typed_phone);
                 add_customer.addBindValue(full_data);
                 add_customer.addBindValue(typed_email);
                 add_customer.addBindValue(QDateTime::currentDateTime());
                 add_customer.addBindValue(connected_id);
                 add_customer.exec();

                 //create an account
                 int id = search_customer_id_by_cpf(typed_cpf);

                 QSqlQuery create_account;
                 create_account.prepare("INSERT INTO account (customer_id, amount, password)"
                                        "VALUES (?,?,?)");
                 create_account.addBindValue(id);
                 create_account.addBindValue(amount);
                 create_account.addBindValue(encrypted_password);
                 create_account.exec();

                 //print a proof
                 //..
             }
             else{ ui->error_message->setText("CPF already registerd"); }
         }
         else { ui->error_message->setText("You're not connected"); }
         close_connection_database();
    }
}

//clean message error when change any filed
void add_customer_account::on_name_input_textChanged() {
    ui->error_message->setText("");
}

void add_customer_account::on_cpf_input_textChanged() {
    ui->error_message->setText("");
}

void add_customer_account::on_phone_input_textChanged() {
    ui->error_message->setText("");
}

void add_customer_account::on_address_input_textChanged() {
    ui->error_message->setText("");
}

void add_customer_account::on_email_input_textChanged() {
    ui->error_message->setText("");
}

void add_customer_account::on_account_password_input_textChanged() {
    ui->error_message->setText("");
}

void add_customer_account::on_account_confirm_passowrd_input_textChanged() {
    ui->error_message->setText("");
}

void add_customer_account::on_amount_input_textChanged() {
    ui->error_message->setText("");
}

