#include "add_staff.h"
#include "ui_add_staff.h"

add_staff::add_staff(QWidget *parent) : QWidget(parent), ui(new Ui::add_staff) {
    ui->setupUi(this);
    ui->cpf_input->setValidator(new QDoubleValidator(0, 999999999999, 2, this)); //allow only numbers. need to be double becuase the range is too long
    ui->phone_input->setValidator(new QDoubleValidator(0, 999999999999, 2, this));

    //date box
    ui->day_input->setRange(1,31);
    ui->month_input->setRange(1,12);
    ui->year_input->setRange(1950,2020);

    //sector option
    ui->sector_input->addItem("");
    ui->sector_input->addItem("Manger");
    ui->sector_input->addItem("Atendant");
}

add_staff::~add_staff() {
    delete ui;
}

//close button
void add_staff::on_pushButton_2_clicked() {
    this->close();
}

//add button
void add_staff::on_pushButton_clicked() {

    QString typed_name             = ui->name_input->text();
    QString typed_cpf              = ui->cpf_input->text();
    QString typed_address          = ui->address_input->text();
    QString typed_phone            = ui->phone_input->text();
    QString typed_day              = ui->day_input->text();
    QString typed_month            = ui->month_input->text();
    QString typed_year             = ui->year_input->text();
    QString typed_email            = ui->email_input->text();
    QString typed_sector           = ui->sector_input->currentText();
    QString typed_password         = ui->password_input->text();
    QString typed_confirm_password = ui->confirm_passowrd_input->text();
    QString full_data = typed_year + "-" + typed_month + "-" + typed_day;

    //verify if all field were filled
    if(typed_name == "" || typed_address == "" || typed_phone == "" || typed_day == "" || typed_month == "" || typed_year == "" ||
       typed_email == "" || typed_sector == "" || typed_password == "" || typed_confirm_password == "") {
        ui->error_message->setText("Enter all needed information.");
    }

    //verify if the typed passowords match
    else if(typed_password != typed_confirm_password) { ui->error_message->setText("Passwords don't match."); }

    //ask for a confimation before sending the message
    else {
        QMessageBox::StandardButton confirmation;
        confirmation = QMessageBox::question(this, "Confirm", "Are you sure?",
                                      QMessageBox::Yes | QMessageBox::No);

        if(confirmation == QMessageBox::No) { /*do nothing*/ }

        //do not make a single "if" because it has two types of errors
        else if(connect_database()) {
             if(search_cpf(typed_cpf)) {  //verify if the staff already exist

                //encrypt password
                Login *ll = new Login();
                QString encrypted_password = ll->encrypt_password(typed_password);

                //add staff
                QSqlQuery add_staff_query;
                add_staff_query.prepare("INSERT INTO staff (full_name, cpf, address, phone_number, birthday, email, sector, password, created_in, by_staff)"
                                        "VALUES (?,?,?,?,?,?,?,?,?,?)");
                add_staff_query.addBindValue(typed_name);
                add_staff_query.addBindValue(typed_cpf);
                add_staff_query.addBindValue(typed_address);
                add_staff_query.addBindValue(typed_phone);
                add_staff_query.addBindValue(full_data);
                add_staff_query.addBindValue(typed_email);
                add_staff_query.addBindValue(typed_sector);
                add_staff_query.addBindValue(encrypted_password);
                add_staff_query.addBindValue(QDateTime::currentDateTime());
                add_staff_query.addBindValue(connected_id.toInt());
                add_staff_query.exec();

                //clean all information
                ui->name_input->setText("");
                ui->cpf_input->setText("");
                ui->address_input->setText("");
                ui->phone_input->setText("");
                ui->day_input->cleanText();
                ui->month_input->cleanText();
                ui->year_input->cleanText();
                ui->email_input ->setText("");
                ui->sector_input ->setCurrentText("");
                ui->password_input ->setText("");
                ui->confirm_passowrd_input ->setText("");

                //show a message that the staff has been added
                QMessageBox::information(this,"About staff", "The staff has been added");
            }
            else{ ui->error_message->setText("CPF already registerd"); }
        }
        else { ui->error_message->setText("You're not connected"); }
        close_connection_database();
    }
}

/*
//QString f = QString(QCryptographicHash::hash(need to pass a variable here),QCryptographicHash::Keccak_512).toHex());
//qDebug() << f;
*/

//clean error message when change any value
void add_staff::on_name_input_textChanged() {
    ui->error_message->setText("");
}

void add_staff::on_cpf_input_textChanged() {
    ui->error_message->setText("");
}

void add_staff::on_phone_input_textChanged() {
    ui->error_message->setText("");
}

void add_staff::on_address_input_textChanged() {
    ui->error_message->setText("");
}

void add_staff::on_email_input_textChanged() {
    ui->error_message->setText("");
}

void add_staff::on_password_input_textChanged() {
    ui->error_message->setText("");
}

void add_staff::on_confirm_passowrd_input_textChanged() {
    ui->error_message->setText("");
}

void add_staff::on_sector_input_currentTextChanged() {
    ui->error_message->setText("");
}

void add_staff::on_day_input_valueChanged() {
    ui->error_message->setText("");
}

void add_staff::on_month_input_valueChanged() {
    ui->error_message->setText("");
}

void add_staff::on_year_input_valueChanged() {
    ui->error_message->setText("");
}
