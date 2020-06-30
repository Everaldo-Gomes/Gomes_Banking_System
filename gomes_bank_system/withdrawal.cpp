#include "withdrawal.h"
#include "ui_withdrawal.h"

withdrawal::withdrawal(QWidget *parent) : QWidget(parent), ui(new Ui::withdrawal) {
    ui->setupUi(this);
    ui->cpf_input->setValidator(new QDoubleValidator(0, 999999999999, 2, this)); //allow only numbers. need to be double becuase the range is too long
    ui->account_input->setValidator(new QDoubleValidator(0, 999999999999, 2, this));
    ui->value_input->setValidator(new QDoubleValidator(0, 999999999999, 2, this));
    ui->status_message->setStyleSheet("QLabel{ color: red;}"); //error message color
    ui->name_input->setFocus();
}

withdrawal::~withdrawal() { delete ui; }

//close button
void withdrawal::on_close_button_clicked() { this->close(); }

//withdrawal button
void withdrawal::on_withdrawal_button_clicked() {

    //check if there is any blank field
    if(ui->name_input->text() == "" || ui->cpf_input->text() == "" || ui->account_input->text() == "" ||
       ui->password_input->text() == "" || ui->value_input->text() == "") { ui->status_message->setText("Enter all needed information"); }

    //check if the value is correct
    else if(!has_decimal_point(ui->value_input->text())) { ui->status_message->setText("The value need to have decimal point. Ex: 50.00"); }
    else {
        //ask for a confirmation
        QMessageBox::StandardButton confirmation;
        confirmation = QMessageBox::question(this, "Confirm", "Are you sure?", QMessageBox::Yes | QMessageBox::No);

        if(confirmation == QMessageBox::No) { /*do nothing*/ }
        else if(connect_database()) {

            //check typed infomation
            QString name = ui->name_input->text();
            QString cpf  = ui->cpf_input->text();
            int account  = ui->account_input->text().toInt();
            bool inf1 = false, acc_exist = false;

            Login *l = new Login();
            QString encrypted_password = l->encrypt_password(ui->password_input->text());

            QSqlQuery info;
            info.prepare("SELECT ct.full_name, ct.cpf, acc.account_number, acc.password FROM customer ct JOIN account acc ON acc.customer_id = ct.id WHERE acc.account_number = ?");
            info.addBindValue(account);
            info.exec();

            while(info.next()) {
                acc_exist = true;
                if(name != info.value(0).toString())                    { ui->status_message->setText("Name does not belong to this account"); }
                else if(cpf != info.value(1).toString())                { ui->status_message->setText("CPF does not belong to this account"); }
                else if(encrypted_password != info.value(3).toString()) { ui->status_message->setText("Password does not match"); }
                else { inf1 = true; }
            }
            if(!acc_exist) { ui->status_message->setText("Account does not exist");  }

            else if(inf1) { //all info are correct

                //verify if the account has enough money
                double current_value = get_account_amount(search_customer_id_by_cpf(cpf)).toDouble(); //get current value
                double amount = ui->value_input->text().toDouble(); //get typed value

                //sub value from the account
                if(current_value - amount >= 0) {
                    add_amount(current_value - amount, cpf); //the function's name is "add" but it used to subtract as well

                    //show a message that the withdrawal has been done and print the receip
                    QMessageBox::information(this,"Withdrawal", "Withdrawal has been done.\nPress Ok to print the withdrawal receipt.");

                    //save withdrawal receipt
                    QSqlQuery save_withdrawal_receipt;
                    save_withdrawal_receipt.prepare("INSERT INTO withdrawal_receipt (customer_id, account_id, amount, responsible_staff_id, withdrawal_day, deposit_receipt_key)"
                                                    "VALUES(?,?,?,?,?,?);");
                    save_withdrawal_receipt.addBindValue(search_customer_id_by_cpf(cpf));
                    save_withdrawal_receipt.addBindValue(get_acc_id(search_customer_id_by_cpf(cpf)));
                    save_withdrawal_receipt.addBindValue(ui->value_input->text());
                    save_withdrawal_receipt.addBindValue(connected_id);
                    save_withdrawal_receipt.addBindValue(QDateTime::currentDateTime());

                    //generate receipt key   AC =account, WD = withdrawal
                    int k;
                    QSqlQuery get_last_id;
                    get_last_id.exec("SELECT id FROM withdrawal_receipt ORDER BY id DESC LIMIT 1;"); //get the last id
                    while(get_last_id.next()) { k = get_last_id.value(0).toInt(); }

                    k *= 5;
                    QString key = "AC" + QString::number(k) + "WD";
                    save_withdrawal_receipt.addBindValue(key);
                    save_withdrawal_receipt.exec();

                    //print receipt
                    QString wirthdrawal_time = QDateTime::currentDateTime().toString();
                    QString file_name = QFileDialog::getSaveFileName((QWidget* )0, "Export PDF", QString(), "*.pdf");
                    if(QFileInfo(file_name).suffix().isEmpty()) { file_name.append(".pdf"); }

                    QPrinter printer(QPrinter::PrinterResolution);
                    printer.setOutputFormat(QPrinter::PdfFormat);
                    printer.setPaperSize(QPrinter::A6);
                    printer.setOutputFileName(file_name);

                    QTextDocument receipt;
                    receipt.setHtml("<pre><h1>   Gomes Bank System</h1>             Withdrawal receipt<br>"
                                    "<p>Name:  "+name+"</p>"
                                    "<p>CPF:   "+cpf+"</p>"
                                    "<p>Account:  "+ui->account_input->text()+"</p>"
                                    "<p>Amount:  R$ "+ui->value_input->text()+"</p>"
                                    "<p>Date:    "+wirthdrawal_time+"</p>"
                                    "<p>Responsible Staff:  "+connected_staff+"</p>"
                                    "<p>Deposit receipt Key:  "+key+"</p>"
                                    "<pre>");
                    receipt.setPageSize(printer.pageRect().size()); //hide the page number
                    receipt.print(&printer);
                    //<pre> preserve the text, now when type tab or space bar the text will contain it

                    //clean all fields
                    ui->name_input->setText("");
                    ui->cpf_input->setText("");
                    ui->account_input->setText("");
                    ui->value_input->setText("");
                    ui->password_input->setText("");
                    ui->status_message->setText("");
                }
                else { ui->status_message->setText("Account does not have enough money"); }
            }
        }
        else { ui->status_message->setText("You're not connected"); }
    }
    close_connection_database();
}

//clean status message when change some value
void withdrawal::on_cpf_input_textChanged()      { ui->status_message->setText(""); }

void withdrawal::on_account_input_textChanged()  { ui->status_message->setText(""); }

void withdrawal::on_value_input_textChanged()    { ui->status_message->setText(""); }

void withdrawal::on_password_input_textChanged() { ui->status_message->setText(""); }

void withdrawal::on_name_input_textChanged()     { ui->status_message->setText(""); }
