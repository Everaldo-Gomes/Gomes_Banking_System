#include "block_customer_account.h"
#include "ui_block_customer_account.h"

extern int found_customer_id;

block_customer_account::block_customer_account(QWidget *parent) : QWidget(parent), ui(new Ui::block_customer_account) {
    ui->setupUi(this);
    ui->cpf_field_input->setValidator(new QDoubleValidator(0, 999999999999, 2, this)); //allow only numbers. need to be double becuase the range is too long
    ui->reason_message_input->setDisabled(true); //disable reason field
    ui->block_button->setDisabled(true); //disable block button
    ui->error_message->setStyleSheet("QLabel{ color: red;}"); //error message color
    ui->cpf_field_input->setFocus();
}

block_customer_account::~block_customer_account() { delete ui; }

//clean all information when change cpf
void block_customer_account::on_cpf_field_input_textChanged() {
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

//close button
void block_customer_account::on_cancel_button_clicked() {
    this->close();
}

//search button
void block_customer_account::on_search_button_clicked() {
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
                ui->block_button->setDisabled(true);
                ui->reason_message_input->setDisabled(true);
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

                found_customer_id = found_id.toInt(); //used when click on block button

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

//block button
void block_customer_account::on_block_button_clicked() {
    QString typed_cpf = ui->cpf_field_input->text();

    //ask for a confirmation
    QMessageBox::StandardButton confirmation;
    confirmation = QMessageBox::question(this, "Confirm", "Are you sure?", QMessageBox::Yes | QMessageBox::No);

    if(confirmation == QMessageBox::No) { /*do nothing*/ }
    else {
        //if the customer/account wasn't blocked yet, put him/her into many_times_customer_blocked database
        if(!blocked_customer_acc_many_times(found_customer_id)) {
            QSqlQuery enter_customer;
            enter_customer.prepare("INSERT INTO many_times_customer_account_blocked (customer_id, times)"
                                   "VALUES (?,?)");
            enter_customer.addBindValue(found_customer_id);
            enter_customer.addBindValue(1);   //in the first time the values of "times" will be 1
            enter_customer.exec();
        }
        else { //just update how many times they were blocked
            int customer_id = search_customer_id_by_cpf(typed_cpf);  qDebug() << "already existed";
            update_qnt_customer_acc_blocked(customer_id);
        }

        //block customer and account
        QString reason_message = ui->reason_message_input->toPlainText();
        block_customer_acc(connected_id, found_customer_id, reason_message);
        QMessageBox::information(this,"About customer & account ", "The customer and his/her account have been blocked"); //show a message that the staff has been blocked

        //clean info
        block_customer_account::on_cpf_field_input_textChanged();
        ui->reason_message_input->setText(""); //this cannot be used when call the function above
        ui->cpf_field_input->setText("");
    }
    close_connection_database();
}

//reason field
void block_customer_account::on_reason_message_input_textChanged() {
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
