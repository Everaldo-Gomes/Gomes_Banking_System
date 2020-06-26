#include "block_staff.h"
#include "ui_block_staff.h"

extern int found_staff_id_int;
extern QString get_sector;

block_staff::block_staff(QWidget *parent) : QWidget(parent), ui(new Ui::block_staff) {
    ui->setupUi(this);
    ui->cpf_field_input->setValidator(new QDoubleValidator(0, 999999999999, 2, this)); //allow only numbers. need to be double becuase the range is too long
    ui->reason_message_input->setDisabled(true); //disable reason field
    ui->block_button->setDisabled(true); //disable block button
    ui->error_message->setStyleSheet("QLabel{ color: red;}"); //error message color
    ui->cpf_field_input->setFocus();
}

block_staff::~block_staff() { delete ui; }

//close button
void block_staff::on_cancel_button_clicked() { this->close(); }

//clean all information when change cpf
void block_staff::on_cpf_field_input_textChanged() {
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
void block_staff::on_search_button_clicked() {
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
                ui->block_button->setDisabled(true);
                ui->reason_message_input->setDisabled(true);
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

                found_staff_id_int = found_id.toInt(); //used when click on block button
                get_sector = found_sector;             //used when click on block button

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

//block button
void block_staff::on_block_button_clicked() {
    QString typed_cpf = ui->cpf_field_input->text();

    //ask for a confirmation
    QMessageBox::StandardButton confirmation;
    confirmation = QMessageBox::question(this, "Confirm", "Are you sure?", QMessageBox::Yes | QMessageBox::No);

    if(confirmation == QMessageBox::No) { /*do nothing*/ }
    else {
        QString reason_message = ui->reason_message_input->toPlainText(); //get the reason

        //check if the staff is trying to block himself
        if(connected_id.toInt() == found_staff_id_int) {
              QMessageBox::information(this,"Blocking not allowed", "You can not block yourself");
        }
        //preventing an attendant from blocking a manager
        else if(get_sector == "Manager" && connected_sector == "Attendant") {
            QMessageBox::information(this,"Blocking not allowed", "You can not block a Manager");
        }
        else {
            //if the staff wasn't blocked yet, put him/her into many_times_staff_blocked database
            if(!blocked_many_times(found_staff_id_int)) {
                QSqlQuery enter_staff;
                enter_staff.prepare("INSERT INTO many_times_staff_blocked (staff_id, times)"
                                    "VALUES (?,?)");
                enter_staff.addBindValue(found_staff_id_int); //staff id
                enter_staff.addBindValue(1);                 //in the first time the values of "times" will be 1
                enter_staff.exec();
            }
            else { //just update how many times they were blocked
                int staff_id = search_id_by_cpf(typed_cpf);
                update_qnt_times_blocked(staff_id);
            }

            //block staff
            block_staff_(connected_id, found_staff_id_int, reason_message);
            QMessageBox::information(this,"About staff", "The staff has been blocked"); //show a message that the staff has been blocked

            //clean info
            block_staff::on_cpf_field_input_textChanged();
            ui->reason_message_input->setText(""); //this cannot be used when call the function above
        }
    }
    close_connection_database();
}

//reason field
void block_staff::on_reason_message_input_textChanged() {
    int max_length = 255;

    //if reach the character limit, delete the following characters
    if(ui->reason_message_input->toPlainText().length() > max_length){
        ui->reason_message_input->textCursor().deletePreviousChar();
    }

    //exhibit the characters limit
    int actual_len = ui->reason_message_input->toPlainText().length();
    int len_to_show = max_length - actual_len;
    ui->character_limit->setText(QString::number(len_to_show));  //convert to string

    //only enable block button when some characters were typed
    if(len_to_show <= 240) ui->block_button->setDisabled(false);
    else { ui->block_button->setDisabled(true); }
}
