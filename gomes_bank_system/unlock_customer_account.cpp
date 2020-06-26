#include "unlock_customer_account.h"
#include "ui_unlock_customer_account.h"

int found_customer_id;

unlock_customer_account::unlock_customer_account(QWidget *parent) : QWidget(parent), ui(new Ui::unlock_customer_account) {
    ui->setupUi(this);
    ui->cpf_field_input->setValidator(new QDoubleValidator(0, 999999999999, 2, this)); //allow only numbers. need to be double becuase the range is too long
    ui->reason_message_input->setDisabled(true); //disable reason field
    ui->unlock_button->setDisabled(true);
    ui->error_message->setStyleSheet("QLabel{ color: red;}"); //error message color
    ui->cpf_field_input->setFocus();
}

unlock_customer_account::~unlock_customer_account() { delete ui; }

//close button
void unlock_customer_account::on_cancel_button_clicked() { this->close(); }

//clean info when change cpf
void unlock_customer_account::on_cpf_field_input_textChanged() {
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
}

//search button
void unlock_customer_account::on_search_button_clicked() {
    QString typed_cpf = ui->cpf_field_input->text();

    if(typed_cpf == "") { ui->error_message->setText("Enter a valid CPF."); }
    else if(connect_database()) {
        if(!search_customer_cpf(typed_cpf)) {
            ui->reason_message_input->setDisabled(false); //enable reason field

            //check if the staff is blocked
            bool is_blocked = customer_blocked(typed_cpf);
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

                found_customer_id = found_id.toInt(); //used when click on unlock button

                if(typed_cpf == found_cpf) {
                    ui->name_output->setText(found_name);
                    ui->cpf_output->setText(found_cpf);
                    ui->address_output ->setText(found_address);
                    ui->phone_output->setText(found_phone);
                    ui->birthday_output->setText(found_birth);
                    ui->email_output->setText(found_email);
                    ui->created_output->setText(found_created_in);
                    ui->account_output->setText(get_acc_number(typed_cpf));
                    ui->id_output->setText(found_id);
                    break;
                }
            }
            ui->account_output->setText(get_acc_number(typed_cpf)); //show account number
        }
        else { ui->error_message->setText("CPF is not registerd"); }
    }
    else { ui->error_message->setText("You re not connected"); }
}

//unlock button
void unlock_customer_account::on_unlock_button_clicked() {
    QMessageBox::StandardButton confirmation;
    confirmation = QMessageBox::question(this, "Confirm", "Are you sure?", QMessageBox::Yes | QMessageBox::No);

    if(confirmation == QMessageBox::No) { /*do nothing*/ }
    else {
        QString typed_cpf = ui->cpf_field_input->text();
        QString reason_message = ui->reason_message_input->toPlainText();
        
        //put info into the unlocked customer dabase
        QSqlQuery unlock_custumer_acc;
        unlock_custumer_acc.prepare("INSERT INTO unlocked_customer_account (responsible_staff_id, unlocked_customer_id, unlocking_day, reason)"
                                  "VALUES (?,?,?,?)");

        unlock_custumer_acc.addBindValue(connected_id.toInt());
        unlock_custumer_acc.addBindValue(found_customer_id);
        unlock_custumer_acc.addBindValue(QDateTime::currentDateTime());
        unlock_custumer_acc.addBindValue(reason_message);
        unlock_custumer_acc.exec();

        //remove the customer from the blocked database
        QSqlQuery remove_from_blocked_table;
        remove_from_blocked_table.prepare("DELETE FROM blocked_customer_account WHERE blocked_customer_id = ?");
        remove_from_blocked_table.addBindValue(found_customer_id);
        remove_from_blocked_table.exec();

        QMessageBox::information(this,"About customer & account", "The customer and his/her account has been unlocked");

        //clean info
        unlock_customer_account::on_cpf_field_input_textChanged();

        //these two cannot be used when call the function above
        ui->reason_message_input->setText("");
        ui->cpf_field_input->setText("");
    }
}

//reason field
void unlock_customer_account::on_reason_message_input_textChanged() {
    int max_length = 255;

    //if reach the character limit, delete the following characters
    if(ui->reason_message_input->toPlainText().length() > max_length){
        ui->reason_message_input->textCursor().deletePreviousChar();
    }

    //exhibit the characters limit
    int actual_len = ui->reason_message_input->toPlainText().length();
    int len_to_show = max_length - actual_len;
    ui->character_limit->setText(QString::number(len_to_show));  //convert to string

    //only enable lock button when some characters were typed
    if(len_to_show <= 240) ui->unlock_button->setDisabled(false);
    else { ui->unlock_button->setDisabled(true); }
}

