#include "change_staff_info.h"
#include "ui_change_staff_info.h"

int found_staff_id;
QString get_sector_;

change_staff_info::change_staff_info(QWidget *parent) : QWidget(parent), ui(new Ui::change_staff_info) {
    ui->setupUi(this);
    ui->cpf_field_input->setValidator(new QDoubleValidator(0, 999999999999, 2, this)); //allow only numbers.
    ui->phone_i_o->setValidator(new QDoubleValidator(0, 999999999999, 2, this));
    ui->change_button->setDisabled(true);
    ui->cpf_field_input->setFocus();

    //sector option
    ui->sector_input->addItem("");
    ui->sector_input->addItem("Manager");
    ui->sector_input->addItem("Attendant");
    ui->sector_input->setDisabled(true);

    //disable changeable fileds
    ui->name_i_o->setDisabled(true);
    ui->address_i_o->setDisabled(true);
    ui->phone_i_o->setDisabled(true);
    ui->sector_input->setDisabled(true);
    ui->current_password_input->setDisabled(true);
    ui->password_input->setDisabled(true);
    ui->confirm_passowrd_input->setDisabled(true);
    ui->reason_message_input->setDisabled(true);

    //error message color
    ui->error_message->setStyleSheet("QLabel{ color: red;}");
    ui->error_message_pw->setStyleSheet("QLabel{ color: red;}");
}

change_staff_info::~change_staff_info() {
    delete ui;
}

//clean all information when change cpf
void change_staff_info::on_cpf_field_input_textChanged() {
    ui->error_message->setText("");
    ui->error_message_pw->setText("");
    ui->name_i_o->setText("");
    ui->cpf_output->setText("");
    ui->address_i_o->setText("");
    ui->phone_i_o->setText("");
    ui->birthday_output->setText("");
    ui->email_i_o->setText("");
    ui->sector_input->setCurrentText("");
    ui->created_output->setText("");
    ui->id_output->setText("");
    ui->status_output->setText("");
    ui->current_password_input->setText("");
    ui->password_input->setText("");
    ui->confirm_passowrd_input->setText("");
    ui->reason_message_input->setText("");
}

//close button
void change_staff_info::on_cancel_button_clicked() {
    this->close();
}

//search button
void change_staff_info::on_search_button_clicked() {
    QString typed_cpf = ui->cpf_field_input->text();

    if(typed_cpf == "") { ui->error_message->setText("Enter a valid CPF"); }
    else if(connect_database()) {
        if(!search_cpf(typed_cpf)) {
            ui->reason_message_input->setDisabled(false); //enable reason fields

            //check if the staff is blocked
            bool is_blocked = staff_blocked(typed_cpf);
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
            get_info.exec("SELECT * FROM staff");

            while(get_info.next()) {
                QString found_id         = get_info.value(0).toString();
                QString found_name       = get_info.value(1).toString();
                QString found_cpf        = get_info.value(2).toString();
                QString found_address    = get_info.value(3).toString();
                QString found_phone      = get_info.value(4).toString();
                QString found_birth      = get_info.value(5).toString();
                QString found_email      = get_info.value(6).toString();
                QString found_sector     = get_info.value(7).toString();
                QString found_created_in = get_info.value(9).toString();

                found_staff_id = found_id.toInt(); //used when click on block button
                get_sector_ = found_sector;        //used when click on block button

                if(typed_cpf == found_cpf) {
                    ui->name_i_o->setText(found_name);
                    ui->cpf_output->setText(found_cpf);
                    ui->address_i_o->setText(found_address);
                    ui->phone_i_o->setText(found_phone);
                    ui->birthday_output->setText(found_birth);
                    ui->email_i_o->setText(found_email);
                    ui->sector_input->setCurrentText(found_sector);
                    ui->created_output->setText(found_created_in);
                    ui->id_output->setText(found_id);
                    ui->sector_input->setDisabled(false);
                    break;
                }
            }

            //enable all changeable fields
            ui->name_i_o->setDisabled(false);
            ui->address_i_o->setDisabled(false);
            ui->phone_i_o->setDisabled(false);
            ui->sector_input->setDisabled(false);
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
void change_staff_info::on_change_button_clicked() {

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

            //check if the staff is trying to change his/her own info
            if(connected_id.toInt() == found_staff_id) {
                  QMessageBox::information(this,"Changing not allowed", "You can not change your own info");
            }
            //preventing an attendant from changing a manager
            else if(get_sector_ == "Manager" && connected_sector == "Attendant") {
                QMessageBox::information(this,"Changing not allowed", "You can not change a Manager");
            }
            else {
                //check the current password
                Login *l = new Login();
                QString encrypted_password = l->encrypt_password(typed_pw);

                if(connect_database()) {
                    bool pw_match = false;

                    //check if the current password match to the password in the database
                    QSqlQuery return_password;
                    return_password.exec("SELECT password FROM staff;");

                    while(return_password.next()) {
                        QString found_pw = return_password.value(0).toString();
                        if(found_pw == encrypted_password) {
                            pw_match = true;
                            break;
                        }
                    }
                    if(pw_match) { //if match, make the changes

                        //put info into changed_staff_info
                        QSqlQuery changes;
                        changes.prepare("INSERT INTO change_staff_info (responsible_staff_id, changed_staff_id, changing_day, reason)"
                                        "VALUES (?,?,?,?)");
                        changes.addBindValue(connected_id.toInt());
                        changes.addBindValue(found_staff_id);
                        changes.addBindValue(QDateTime::currentDateTime());
                        changes.addBindValue(reason_message);
                        changes.exec();

                        //get the last id from changed_staff_info
                        int id;
                        QSqlQuery get_last_id;
                        get_last_id.exec("SELECT id FROM change_staff_info ORDER BY id desc LIMIT 1;");

                        while(get_last_id.next()) { id = get_last_id.value(0).toInt(); }

                        //get changeable info from staff
                        QSqlQuery get_current_staff_info;
                        get_current_staff_info.prepare("SELECT * FROM staff WHERE id = ?");
                        get_current_staff_info.addBindValue(found_staff_id);
                        get_current_staff_info.exec();

                        while(get_current_staff_info.next()) {
                            QString found_name      = get_current_staff_info.value(1).toString();
                            QString found_address   = get_current_staff_info.value(3).toString();
                            QString found_phone     = get_current_staff_info.value(4).toString();
                            QString found_email     = get_current_staff_info.value(6).toString();
                            QString found_sector    = get_current_staff_info.value(7).toString();
                            QString found_password  = get_current_staff_info.value(8).toString();

                            //put the changeable found info into previous_staff_info database
                            if(encrypted_password == found_password) {
                                QSqlQuery store_staff_previous_info;
                                store_staff_previous_info.prepare("INSERT INTO previous_staff_information (change_table_id, full_name, address, phone_number, email, sector, password)"
                                                                  "VALUES (?,?,?,?,?,?,?)");

                                store_staff_previous_info.addBindValue(id);
                                store_staff_previous_info.addBindValue(found_name);
                                store_staff_previous_info.addBindValue(found_address);
                                store_staff_previous_info.addBindValue(found_phone);
                                store_staff_previous_info.addBindValue(found_email);
                                store_staff_previous_info.addBindValue(found_sector);
                                store_staff_previous_info.addBindValue(found_password);
                                store_staff_previous_info.exec();
                                break;
                           }
                        }
                        //changing info
                        QSqlQuery change_info;
                        change_info.prepare("UPDATE staff SET full_name = ?, address = ?, phone_number = ?, email = ?, sector = ?, password = ? WHERE id = ?");
                        change_info.addBindValue(ui->name_i_o->text());
                        change_info.addBindValue(ui->address_i_o->text());
                        change_info.addBindValue(ui->phone_i_o->text());
                        change_info.addBindValue(ui->email_i_o->text());
                        change_info.addBindValue(ui->sector_input->currentText());

                        //if a new password was inserted replace the old one
                        if(ui->password_input->text() != "" && ui->confirm_passowrd_input->text() != "") {
                            QString new_pw = l->encrypt_password(ui->password_input->text());
                            change_info.addBindValue(new_pw);
                        }
                        else {//keep the old password
                            QString old_pw = l->encrypt_password(ui->current_password_input->text());
                            change_info.addBindValue(old_pw);
                        }
                        change_info.addBindValue(found_staff_id);  //where query parameter
                        change_info.exec();

                        //show a message that the staff's info has been changed
                        QMessageBox::information(this,"Change staff", "The staff's info has been changed");

                        //clean all info
                        change_staff_info::on_cpf_field_input_textChanged();
                    }
                    else { ui->error_message_pw->setText("Current passwords is wrong"); }
                }
            }
        }
    }
    close_connection_database();
}

//reason field
void change_staff_info::on_reason_message_input_textChanged() {
    int max_length = 255;

    //if reach the character limit delete the following characters
    if(ui->reason_message_input->toPlainText().length() > max_length){
        ui->reason_message_input->textCursor().deletePreviousChar();
    }

    //exhibit the characters limit on the screen
    int actual_len = ui->reason_message_input->toPlainText().length();
    int len_to_show = max_length - actual_len;
    ui->character_limit->setText(QString::number(len_to_show));  //convert to string

    //only enable block button when some characters were typed
    if(len_to_show <= 240) ui->change_button->setDisabled(false);
    else { ui->change_button->setDisabled(true); }
}

//clean error message_pw when typed
void change_staff_info::on_password_input_textChanged() {
    ui->error_message_pw->setText("");
}

void change_staff_info::on_confirm_passowrd_input_textChanged() {
    ui->error_message_pw->setText("");
}

void change_staff_info::on_current_password_input_textChanged() {
       ui->error_message_pw->setText("");
}
