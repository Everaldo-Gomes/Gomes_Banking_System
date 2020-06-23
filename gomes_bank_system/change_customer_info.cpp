#include "change_customer_info.h"
#include "ui_change_customer_info.h"

int customer_id;

change_customer_info::change_customer_info(QWidget *parent) : QWidget(parent), ui(new Ui::change_customer_info) {
    ui->setupUi(this);
    ui->cpf_field_input->setValidator(new QDoubleValidator(0, 999999999999, 2, this)); //allow only numbers.
    ui->phone_i_o->setValidator(new QDoubleValidator(0, 999999999999, 2, this));
    ui->change_button->setDisabled(true);
    ui->cpf_field_input->setFocus();

    //disable changeable fileds
    ui->name_i_o->setDisabled(true);
    ui->address_i_o->setDisabled(true);
    ui->phone_i_o->setDisabled(true);
    ui->current_password_input->setDisabled(true);
    ui->password_input->setDisabled(true);
    ui->confirm_passowrd_input->setDisabled(true);
    ui->reason_message_input->setDisabled(true);

    //error message color
    ui->error_message->setStyleSheet("QLabel{ color: red;}");
    ui->error_message_pw->setStyleSheet("QLabel{ color: red;}");
}

change_customer_info::~change_customer_info() {
    delete ui;
}

//clean all information when change cpf
void change_customer_info::on_cpf_field_input_textChanged() {
    ui->error_message->setText("");
    ui->error_message_pw->setText("");
    ui->name_i_o->setText("");
    ui->cpf_output->setText("");
    ui->address_i_o->setText("");
    ui->phone_i_o->setText("");
    ui->birthday_output->setText("");
    ui->email_i_o->setText("");
    ui->created_output->setText("");
    ui->id_output->setText("");
    ui->account_output->setText("");
    ui->status_output->setText("");
    ui->current_password_input->setText("");
    ui->password_input->setText("");
    ui->confirm_passowrd_input->setText("");
    ui->reason_message_input->setText("");
}

//cancel button
void change_customer_info::on_cancel_button_clicked() {
    this->close();
}

//search button
void change_customer_info::on_search_button_clicked() {
    QString typed_cpf = ui->cpf_field_input->text();

    if(typed_cpf == "") { ui->error_message->setText("Enter a valid CPF"); }
    else if(connect_database()) {
        if(!search_customer_cpf(typed_cpf)) {
            ui->reason_message_input->setDisabled(false); //enable reason fields

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
                QString found_id         = get_info.value(0).toString();
                QString found_name       = get_info.value(1).toString();
                QString found_cpf        = get_info.value(2).toString();
                QString found_address    = get_info.value(3).toString();
                QString found_phone      = get_info.value(4).toString();
                QString found_birth      = get_info.value(5).toString();
                QString found_email      = get_info.value(6).toString();
                QString found_created_in = get_info.value(7).toString();

                customer_id = found_id.toInt(); //used when click on block button

                if(typed_cpf == found_cpf) {
                    ui->name_i_o->setText(found_name);
                    ui->cpf_output->setText(found_cpf);
                    ui->address_i_o->setText(found_address);
                    ui->phone_i_o->setText(found_phone);
                    ui->birthday_output->setText(found_birth);
                    ui->email_i_o->setText(found_email);
                    ui->created_output->setText(found_created_in);
                    ui->id_output->setText(found_id);
                    ui->account_output->setText(get_acc_number(typed_cpf));
                    break;
                }
            }

            //enable all changeable fields
            ui->name_i_o->setDisabled(false);
            ui->address_i_o->setDisabled(false);
            ui->phone_i_o->setDisabled(false);
            ui->current_password_input->setDisabled(false);
            ui->password_input->setDisabled(false);
            ui->confirm_passowrd_input->setDisabled(false);
            ui->reason_message_input->setDisabled(false);
        }
        else {
            ui->error_message->setStyleSheet("QLabel{ color: red;}");
            ui->error_message->setText("CPF is not registerd");
        }
    }
    else {
        ui->error_message->setStyleSheet("QLabel{ color: red;}");
        ui->error_message->setText("You re not connected");
    }
}

//change button
void change_customer_info::on_change_button_clicked() {
    QString typed_cpf = ui->cpf_field_input->text();

    //verify if the new passoword match
    if(ui->password_input->text() != ui->confirm_passowrd_input->text()) {
        ui->error_message_pw->setText("New passwords don't match");
    }
    else {
        //confirmation box
        QMessageBox::StandardButton confirmation;
        confirmation = QMessageBox::question(this, "Confirm", "Are you sure?",
                                      QMessageBox::Yes | QMessageBox::No);

        if(confirmation == QMessageBox::No) { /*do nothing*/ }
        else {
            QString reason_message = ui->reason_message_input->toPlainText();
            QString typed_pw = ui->current_password_input->text();

            //check the current password
            Login *l = new Login();
            QString encrypted_password = l->encrypt_password(typed_pw);

            if(connect_database()) {
                bool pw_match = false;

                //check if the current password match to the password in the database
                QSqlQuery return_password;
                return_password.exec("SELECT password FROM account;");

                while(return_password.next()) {
                    QString found_pw = return_password.value(0).toString();
                    if(found_pw == encrypted_password) {
                        pw_match = true;
                        break;
                    }
                }
                if(pw_match) { //if match, make the changes

                    //put info into the changed_customer_info
                    QSqlQuery changes;
                    changes.prepare("INSERT INTO change_customer_info (responsible_staff_id, changed_customer_id, changing_day, reason)"
                                    "VALUES (?,?,?,?)");
                    changes.addBindValue(connected_id.toInt());
                    changes.addBindValue(customer_id);
                    changes.addBindValue(QDateTime::currentDateTime());
                    changes.addBindValue(reason_message);
                    changes.exec();

                    //get the last id from changed_customer_info
                    int id;
                    QSqlQuery get_last_id;
                    get_last_id.exec("SELECT id FROM change_customer_info ORDER BY id DESC LIMIT 1;");

                    while(get_last_id.next()) { id = get_last_id.value(0).toInt(); }

                    //get changeable info from staff
                    QSqlQuery get_current_customer_info;
                    get_current_customer_info.prepare("SELECT * FROM customer WHERE id = ?");
                    get_current_customer_info.addBindValue(customer_id);
                    get_current_customer_info.exec();

                    //get account password
                    QString found_password;
                    QSqlQuery get_acc_pw;
                    get_acc_pw.exec("SELECT an.password FROM account an JOIN customer ct ON ct.id = an.customer_id WHERE ct.cpf = '"+typed_cpf+"';");
                    while(get_acc_pw.next()) { found_password = get_acc_pw.value(0).toString(); }

                    while(get_current_customer_info.next()) {
                        QString found_name      = get_current_customer_info.value(1).toString();
                        QString found_address   = get_current_customer_info.value(3).toString();
                        QString found_phone     = get_current_customer_info.value(4).toString();
                        QString found_email     = get_current_customer_info.value(6).toString();

                        //put the changeable found info into previous_customer_info database
                        if(encrypted_password == found_password) {

                            QSqlQuery store_staff_previous_info;
                            store_staff_previous_info.prepare("INSERT INTO previous_customer_information (change_table_id, full_name, address, phone_number, email, password)"
                                                              "VALUES (?,?,?,?,?,?)");
                            store_staff_previous_info.addBindValue(id);
                            store_staff_previous_info.addBindValue(found_name);
                            store_staff_previous_info.addBindValue(found_address);
                            store_staff_previous_info.addBindValue(found_phone);
                            store_staff_previous_info.addBindValue(found_email);
                            store_staff_previous_info.addBindValue(found_password);
                            store_staff_previous_info.exec();
                            break;
                       }
                    }

                    //changing info
                    QSqlQuery change_info;
                    change_info.prepare("UPDATE customer SET full_name = ?, address = ?, phone_number = ?, email = ? WHERE id = ?");
                    change_info.addBindValue(ui->name_i_o->text());
                    change_info.addBindValue(ui->address_i_o->text());
                    change_info.addBindValue(ui->phone_i_o->text());
                    change_info.addBindValue(ui->email_i_o->text());
                    change_info.addBindValue(customer_id);
                    change_info.exec();

                    //change account password
                    QSqlQuery change_pw;
                    change_pw.prepare("UPDATE account SET password = ? WHERE customer_id = ?");

                    //if a new password was inserted replace the old one
                    if(ui->password_input->text() != "" && ui->confirm_passowrd_input->text() != "") {
                        QString new_pw = l->encrypt_password(ui->password_input->text());
                        change_pw.addBindValue(new_pw);
                    }
                    else {//keep the old password
                        QString old_pw = l->encrypt_password(ui->current_password_input->text());
                        change_pw.addBindValue(old_pw);
                    }
                    change_pw.addBindValue(customer_id);  //where query parameter
                    change_pw.exec();

                    //show a message that the customer's info has been changed
                    QMessageBox::information(this,"Change customer", "The customer's info has been changed");

                    //clean all info
                    change_customer_info::on_cpf_field_input_textChanged();
                }
                else { ui->error_message_pw->setText("Current passwords is wrong"); }
            }
        }
    }
    close_connection_database();
}

//reason field
void change_customer_info::on_reason_message_input_textChanged() {
    int max_length = 255;

    //if reach the character limit, delete the following characters
    if(ui->reason_message_input->toPlainText().length() > max_length){
        ui->reason_message_input->textCursor().deletePreviousChar();
    }

    //exhibit the characters limit
    int actual_len = ui->reason_message_input->toPlainText().length();
    int len_to_show = max_length - actual_len;
    ui->character_limit->setText(QString::number(len_to_show));  //convert to string

    //only enable change button when some characters were typed
    if(len_to_show <= 240) ui->change_button->setDisabled(false);
    else { ui->change_button->setDisabled(true); }
}


//clean password message error when type on password field
void change_customer_info::on_current_password_input_textChanged() {
    ui->error_message_pw->setText("");
}

void change_customer_info::on_password_input_textChanged() {
    ui->error_message_pw->setText("");
}

void change_customer_info::on_confirm_passowrd_input_textChanged() {
    ui->error_message_pw->setText("");
}
