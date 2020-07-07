#include "transfer.h"
#include "ui_transfer.h"

transfer::transfer(QWidget *parent) : QWidget(parent), ui(new Ui::transfer) {
    ui->setupUi(this);
    ui->cpf_from_input->setValidator(new QDoubleValidator(0, 999999999999, 2, this));
    ui->value_from_input->setValidator(new QDoubleValidator(0, 999999999999, 2, this));
    ui->cpf_to_input->setValidator(new QDoubleValidator(0, 999999999999, 2, this));
    ui->account_to_input->setValidator(new QDoubleValidator(0, 999999999999, 2, this));
    ui->account_from_input->setValidator(new QDoubleValidator(0, 999999999999, 2, this));
    ui->status_message->setStyleSheet("QLabel{ color: red;}");
}

transfer::~transfer() { delete ui; }

//close button
void transfer::on_close_button_clicked() { this->close(); }

//transfer button
void transfer::on_transfer_button_clicked() {

    //check if there is any blank field
    if(ui->name_from_input->text() == "" || ui->cpf_from_input->text()   == "" || ui->account_from_input->text() == "" ||
       ui->password_input->text()  == "" || ui->value_from_input->text() == "" || ui->name_to_input->text()      == "" ||
       ui->cpf_to_input->text()    == "" || ui->account_to_input->text() == "") { ui->status_message->setText("Enter all needed information"); }

    //check if the value is correct
    else if(!has_decimal_point(ui->value_from_input->text())) { ui->status_message->setText("The value need to have decimal point. Ex: 50.00"); }
    else {
        //ask for a confirmation
        QMessageBox::StandardButton confirmation;
        confirmation = QMessageBox::question(this, "Confirm", "Are you sure?", QMessageBox::Yes | QMessageBox::No);

        if(confirmation == QMessageBox::No) { /*do nothing*/ }
        else if(connect_database()) {

            //check typed infomation
            QString from_name = ui->name_from_input->text();
            QString to_name   = ui->name_to_input->text();
            QString from_cpf  = ui->cpf_from_input->text();
            QString to_cpf    = ui->cpf_to_input->text();
            int from_account  = ui->account_from_input->text().toInt();
            int to_account    = ui->account_to_input->text().toInt();
            bool from_inf = false, to_inf = false, from_acc_exist = false, to_acc_exist = false;

            //preventing from transfer to the same customer. Ex From A to A
            if(from_cpf == to_cpf) { ui->status_message->setText("Cannot transfer to the same account"); }
            else {
                Login *l = new Login();
                QString encrypted_password = l->encrypt_password(ui->password_input->text());

                //check info FROM
                QSqlQuery from_info;
                from_info.prepare("SELECT ct.full_name, ct.cpf, acc.account_number, acc.password FROM customer ct JOIN account acc ON acc.customer_id = ct.id WHERE acc.account_number = ?");
                from_info.addBindValue(from_account);
                from_info.exec();

                while(from_info.next()) {
                    from_acc_exist = true;
                    if(from_name != from_info.value(0).toString())               { ui->status_message->setText("FROM: Name does not belong to this account"); }
                    else if(from_cpf != from_info.value(1).toString())           { ui->status_message->setText("FROM: CPF does not belong to this account"); }
                    else if(encrypted_password != from_info.value(3).toString()) { ui->status_message->setText("FROM: Password does not match"); }
                    else { from_inf = true; }
                }
                if(!from_acc_exist) { ui->status_message->setText("FROM: Account does not exist");  }

                //check info TO
                QSqlQuery to_info;
                to_info.prepare("SELECT ct.full_name, ct.cpf, acc.account_number FROM customer ct JOIN account acc ON acc.customer_id = ct.id WHERE acc.account_number = ?");
                to_info.addBindValue(to_account);
                to_info.exec();

                while(to_info.next()) {
                    to_acc_exist = true;
                    if(to_name != to_info.value(0).toString())                 { ui->status_message->setText("TO: Name does not belong to this account"); }
                    else if(to_cpf != to_info.value(1).toString())             { ui->status_message->setText("TO: CPF does not belong to this account"); }
                    else { to_inf = true; }
                }
                if(!to_acc_exist) { ui->status_message->setText("TO: Account does not exist");  }

                else if(from_inf && to_inf) { //all info are correct

                    //show a message that the transaction has been done and print the transfer voucher
                    QMessageBox::information(this,"Transaction", "Transaction has been done.\nPress Ok to print the transfer voucher.");

                    //check if the account whose will sent money, has enough money
                    double from_current_value = get_account_amount(search_customer_id_by_cpf(from_cpf)).toDouble(); //get current value
                    double from_amount = ui->value_from_input->text().toDouble();

                    if(from_current_value - from_amount < 0) { ui->status_message->setText("Account does not have enough money"); }
                    else {  //transfer

                        //subtract value from the account which will send money
                        add_amount(from_current_value - from_amount, from_cpf); //the function's name is "add" but it used to subtract as well

                        //add value to the favoured account
                        double to_current_value = get_account_amount(search_customer_id_by_cpf(to_cpf)).toDouble(); //get current value
                        double to_amount = ui->value_from_input->text().toDouble();
                        add_amount(to_current_value + to_amount, to_cpf);

                        //save info into the database
                        QSqlQuery save_transaction;
                        save_transaction.prepare("INSERT INTO transfer_voucher (from_customer_id, from_account_id, amount, to_customer_id, to_account_id, responsible_staff_id, transfer_day, transfer_voucher_key)"
                                                 "VALUES(?,?,?,?,?,?,?,?);");
                        save_transaction.addBindValue(search_customer_id_by_cpf(from_cpf));
                        save_transaction.addBindValue(get_acc_id(search_customer_id_by_cpf(from_cpf)));
                        save_transaction.addBindValue(from_amount);
                        save_transaction.addBindValue(search_customer_id_by_cpf(to_cpf));
                        save_transaction.addBindValue(get_acc_id(search_customer_id_by_cpf(to_cpf)));
                        save_transaction.addBindValue(connected_id);

                        QString transaction_time = QDateTime::currentDateTime().toString(); //used to put in the transfer voucher
                        save_transaction.addBindValue(QDateTime::currentDateTime());

                        //generate transfer voucher key   AC =account, TF = transfer
                        int k;
                        QSqlQuery get_last_id;
                        get_last_id.exec("SELECT id FROM transfer_voucher ORDER BY id DESC LIMIT 1;"); //get the last id
                        while(get_last_id.next()) { k = get_last_id.value(0).toInt(); }

                        k *= 4;
                        QString key = "AC" + QString::number(k) + "TF";
                        save_transaction.addBindValue(key);
                        save_transaction.exec();

                        //print transfer voucher
                        QString file_name = QFileDialog::getSaveFileName((QWidget* )0, "Export PDF", QString(), "*.pdf");
                           if (QFileInfo(file_name).suffix().isEmpty()) { file_name.append(".pdf"); }

                        QPrinter printer(QPrinter::PrinterResolution);
                        printer.setOutputFormat(QPrinter::PdfFormat);
                        printer.setPaperSize(QPrinter::A6);
                        printer.setOutputFileName(file_name);

                        QTextDocument receipt;
                        receipt.setHtml("<pre><h1>   Gomes Bank System</h1>             Transfer voucher<br><br>"
                                        "FROM"
                                        "<p>Name: "+from_name+"</p>"
                                        "<p>CPF: "+from_cpf+"</p>"
                                        "<p>Account: "+ui->account_from_input->text()+"</p>"
                                        "<p>Amount: R$ "+ui->value_from_input->text()+"</p><br>"
                                        "TO"
                                        "<p>Name: "+to_name+"</p>"
                                        "<p>CPF: "+to_cpf+"</p>"
                                        "<p>Account: "+ui->account_to_input->text()+"</p>"
                                        "<p>Date: "+transaction_time+"</p>"
                                        "<p>Responsible Staff: "+connected_staff+"</p>"
                                        "<p>Deposit receipt Key: "+key+"</p>"
                                        "<pre>");
                        receipt.setPageSize(printer.pageRect().size()); //hide the page number
                        receipt.print(&printer);
                        //<pre> preserve the text, now when type tab or space bar the text will contain it

                        //clean all fields
                        ui->name_from_input->setText("");
                        ui->cpf_from_input->setText("");
                        ui->value_from_input->setText("");
                        ui->account_from_input->setText("");
                        ui->password_input->setText("");
                        ui->name_to_input->setText("");
                        ui->cpf_to_input->setText("");
                        ui->account_to_input->setText("");
                    }
                }
            }
        }
        else { ui->status_message->setText("You're not connected"); }
    }
    close_connection_database();
}

//clean status message when type
void transfer::on_name_from_input_textChanged()    { ui->status_message->setText(""); }

void transfer::on_cpf_from_input_textChanged()     { ui->status_message->setText(""); }

void transfer::on_account_from_input_textChanged() { ui->status_message->setText(""); }

void transfer::on_value_from_input_textChanged()   { ui->status_message->setText(""); }

void transfer::on_password_input_textChanged()     { ui->status_message->setText(""); }

void transfer::on_name_to_input_textChanged()      { ui->status_message->setText(""); }

void transfer::on_cpf_to_input_textChanged()       { ui->status_message->setText(""); }

void transfer::on_account_to_input_textChanged()   { ui->status_message->setText(""); }
