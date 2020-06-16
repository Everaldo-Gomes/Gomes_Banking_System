#include "unlock_staff.h"
#include "ui_unlock_staff.h"

int found_staff_id_int;

unlock_staff::unlock_staff(QWidget *parent) : QWidget(parent), ui(new Ui::unlock_staff) {
    ui->setupUi(this);
    ui->cpf_field_input->setValidator(new QDoubleValidator(0, 999999999999, 2, this)); //allow only numbers. need to be double becuase the range is too long
    ui->reason_message_input->setDisabled(true); //disable reason field
    ui->unlock_button->setDisabled(true); //disable unlock button
}

unlock_staff::~unlock_staff() {
    delete ui;
}

//clean all information
void unlock_staff::on_cpf_field_input_textChanged() {
    ui->error_message->setText("");
    ui->name_output->setText("");
    ui->cpf_output->setText("");
    ui->address_output ->setText("");
    ui->phone_output->setText("");
    ui->birthday_output->setText("");
    ui->email_output->setText("");
    ui->sector_output->setText("");
    ui->created_output->setText("");
    ui->id_output->setText("");
    ui->status_output->setText("");
}

//search button
void unlock_staff::on_search_button_clicked() {
    QString typed_cpf = ui->cpf_field_input->text();

    if(typed_cpf == "") { ui->error_message->setText("Enter a valid CPF."); }
    else if(connect_database()) {
        if(!search_cpf(typed_cpf)) {
            ui->reason_message_input->setDisabled(false); //enable reason field

            //check if the staff is blocked
            bool is_blocked = staff_blocked(typed_cpf);
            if(is_blocked) {
                ui->status_output->setText("Blocked");
                ui->status_output->setStyleSheet("QLabel{ color: red;}");
                ui->unlock_button->setDisabled(true);
                ui->reason_message_input->setDisabled(false);
            }
            else {
                ui->status_output->setText("Unlocked");
                ui->status_output->setStyleSheet("QLabel{ color: green;}");
                ui->unlock_button->setDisabled(true);
                ui->reason_message_input->setDisabled(true);
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

                found_staff_id_int = found_id.toInt(); //used when clink on block button

                if(typed_cpf == found_cpf) {
                    ui->name_output->setText(found_name);
                    ui->cpf_output->setText(found_cpf);
                    ui->address_output ->setText(found_address);
                    ui->phone_output->setText(found_phone);
                    ui->birthday_output->setText(found_birth);
                    ui->email_output->setText(found_email);
                    ui->sector_output->setText(found_sector);
                    ui->created_output->setText(found_created_in);
                    ui->id_output->setText(found_id);
                    break;
                }
            }
        }
        else { ui->error_message->setText("CPF is not registerd"); }
    }
    else { ui->error_message->setText("You re not connected"); }
}

//unlock button
void unlock_staff::on_unlock_button_clicked() {

    QMessageBox::StandardButton confirmation;
    confirmation = QMessageBox::question(this, "Confirm", "Are you sure?",
                                  QMessageBox::Yes | QMessageBox::No);

    if(confirmation == QMessageBox::No) { /*do nothing*/ }
    else {
        QString typed_cpf = ui->cpf_field_input->text();
        QString reason_message = ui->reason_message_input->toPlainText();

        //check if the staff is trying to get yourself unlocked
        if(connected_id.toInt() == found_staff_id_int) {
              QMessageBox::information(this,"Unlocking not allowed", "You can not unblock yourself");
        }
        else {
            //put info into the unlocked dabase
            QSqlQuery unlock_staff_query;
            unlock_staff_query.prepare("INSERT INTO unlocked_staff (responsable_staff_id, unlocked_staff_id, unlocking_day, reason)"
                                      "VALUES (?,?,?,?)");

            unlock_staff_query.addBindValue(connected_id.toInt());
            unlock_staff_query.addBindValue(found_staff_id_int);
            unlock_staff_query.addBindValue(QDateTime::currentDateTime());
            unlock_staff_query.addBindValue(reason_message);
            unlock_staff_query.exec();

            //remove the staff from the blocked database
            QSqlQuery remove_from_blocked_table;
            remove_from_blocked_table.prepare("DELETE FROM blocked_staff WHERE blocked_staff_id = ?");
            remove_from_blocked_table.addBindValue(found_staff_id_int);
            remove_from_blocked_table.exec();

            QMessageBox::information(this,"About staff", "The staff has been unlocked"); //show a message that the staff has been unlocked

            //clean info
            ui->error_message->setText("");
            ui->name_output->setText("");
            ui->cpf_output->setText("");
            ui->address_output->setText("");
            ui->phone_output->setText("");
            ui->birthday_output->setText("");
            ui->email_output->setText("");
            ui->sector_output->setText("");
            ui->created_output->setText("");
            ui->id_output->setText("");
            ui->status_output->setText("");
            ui->reason_message_input->setText("");
        }
    }
    close_connection_database();
}

//reason field
void unlock_staff::on_reason_message_input_textChanged() {
    int max_length = 255;

    //if reach the character limit delete the following characters
    if(ui->reason_message_input->toPlainText().length() > max_length){
        ui->reason_message_input->textCursor().deletePreviousChar();
    }

    //exhibit the characters limit on the screen
    int actual_len = ui->reason_message_input->toPlainText().length();
    int len_to_show = max_length - actual_len;
    ui->character_limit->setText(QString::number(len_to_show));  //convert to string

    //only enable unlock button when some characters were typed
    if(len_to_show <= 240) ui->unlock_button->setDisabled(false);
    else { ui->unlock_button->setDisabled(true); }
}

//cancel button
void unlock_staff::on_cancel_button_clicked() {
    this->close();
}
