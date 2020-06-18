#include "change_staff_info.h"
#include "ui_change_staff_info.h"

//int found_staff_id_int;

change_staff_info::change_staff_info(QWidget *parent) : QWidget(parent), ui(new Ui::change_staff_info) {
    ui->setupUi(this);
    ui->cpf_field_input->setValidator(new QDoubleValidator(0, 999999999999, 2, this)); //allow only numbers.
    ui->change_button->setDisabled(true);

    //sector option
    ui->sector_input->addItem("");
    ui->sector_input->addItem("Manger");
    ui->sector_input->addItem("Atendant");
    ui->sector_input->setDisabled(true);
}

change_staff_info::~change_staff_info() {
    delete ui;
}

//clean all information when change cpf
void change_staff_info::on_cpf_field_input_textChanged() {
    ui->error_message->setText("");
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

    if(typed_cpf == "") { ui->error_message->setText("Enter a valid CPF."); }
    else if(connect_database()) {
        if(!search_cpf(typed_cpf)) {
            ui->reason_message_input->setDisabled(false); //enable reason field

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

                //found_staff_id_int = found_id.toInt(); //used when clink on block button

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
        }
        else { ui->error_message->setText("CPF is not registerd"); }
    }
    else { ui->error_message->setText("You re not connected"); }
}

//change button
void change_staff_info::on_change_button_clicked() {
    QString typed_pw = ui->current_password_input->text();
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
        //if match, make the changees
        if(pw_match) {

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
