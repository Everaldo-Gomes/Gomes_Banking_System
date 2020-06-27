#include "deposit.h"
#include "ui_deposit.h"

int cust_id;
double value;
QString cust_cpf;

deposit::deposit(QWidget *parent) : QWidget(parent), ui(new Ui::deposit) {
    ui->setupUi(this);
    ui->depositor_cpf_input->setValidator(new QDoubleValidator(0, 999999999999, 2, this)); //allow only numbers. need to be double becuase the range is too long
    ui->favoured_account_input->setValidator(new QDoubleValidator(0, 999999999999, 2, this)); //allow only numbers. need to be double becuase the range is too long
    ui->value_input->setValidator(new QDoubleValidator(0, 999999999999, 2, this)); //allow only numbers. need to be double becuase the range is too long
    ui->error_message->setStyleSheet("QLabel{ color: red;}"); //error message color
    ui->deposit_button->setDisabled(true);
}

deposit::~deposit() { delete ui; }

//close button
void deposit::on_cancel_button_clicked() { this->close(); }

//check button
void deposit::on_check_button_clicked() {

    //get what was typed
    QString typed_name    = ui->depositor_name_input->text();
    QString typed_cpf     = ui->depositor_cpf_input->text();
    QString typed_account = ui->favoured_account_input->text();
    QString typed_value   = ui->value_input->text();
            value         = typed_value.toDouble();

    if(typed_name == "" || typed_cpf == "" || typed_account == "" || typed_value == "") { ui->error_message->setText("Enter all needed information"); }
    else if(connect_database()) {

        //get destination information
        QSqlQuery get_destination_info;
        get_destination_info.prepare("SELECT ct.full_name, ct.cpf, acc.account_number, ct.id FROM customer ct JOIN account acc ON acc.customer_id = ct.id WHERE acc.account_number = ?");
        get_destination_info.addBindValue(typed_account);
        get_destination_info.exec();

        while(get_destination_info.next()) {
            ui->name_output->setText(get_destination_info.value(0).toString());

            cust_cpf = get_destination_info.value(1).toString();
            ui->cpf_output->setText(get_destination_info.value(1).toString());

            ui->account_output->setText(get_destination_info.value(2).toString());
            cust_id = get_destination_info.value(3).toInt();

            ui->deposit_button->setDisabled(false);
        }
        //check if found the account
        if(ui->account_output->text() == "") { ui->error_message->setText("Account was not found"); }

        //check if the value is correct
        if(!has_decimal_point(typed_value)) { ui->error_message->setText("The value need to have decimal point. Ex: 50.00"); }
    }
    else { ui->error_message->setText("You're not connected"); }
    close_connection_database();
}

//deposit button
void deposit::on_deposit_button_clicked() {

    //ask for a confirmation
    QMessageBox::StandardButton confirmation;
    confirmation = QMessageBox::question(this, "Confirm", "Are you sure?", QMessageBox::Yes | QMessageBox::No);

    if(confirmation == QMessageBox::No) { /*do nothing*/ }
    else if(connect_database()) {

        QString amount = get_account_amount(cust_id); //get the current amount
        double current_value = amount.toDouble();
        add_amount(value + current_value, cust_cpf); //add the value in the account

        //show a message that the deposit has been done and print the receipt
        QMessageBox::information(this,"Deposit", "The deposit has been done.\nPress Ok to print the deposit receipt.");

        //save info into the database
        QSqlQuery save_deposit_receipt;
        save_deposit_receipt.prepare("INSERT INTO deposit_receipt (customer_id, account_id, amount, who_did_name, who_did_cpf, responsible_staff_id, deposit_day, deposit_receipt_key)"
                                     "VALUES(?,?,?,?,?,?,?,?)");
        save_deposit_receipt.addBindValue(cust_id);
        save_deposit_receipt.addBindValue(get_acc_id(search_customer_id_by_cpf(cust_cpf)));
        save_deposit_receipt.addBindValue(value);
        save_deposit_receipt.addBindValue(ui->depositor_name_input->text());
        save_deposit_receipt.addBindValue(ui->depositor_cpf_input->text());
        save_deposit_receipt.addBindValue(connected_id);

        //need to store in this variable because it will be used when print the receipt
        QString deposit_time = QDateTime::currentDateTime().toString();
        save_deposit_receipt.addBindValue(QDateTime::currentDateTime());

        //generate receipt key   AC =account, DE = deposit
        int k;
        QSqlQuery get_last_id;
        get_last_id.exec("SELECT id FROM deposit_receipt ORDER BY id DESC LIMIT 1;"); //get the last id
        while(get_last_id.next()) { k = get_last_id.value(0).toInt(); }
        k *= 3;
        QString key = "AC" + QString::number(k) + "DE";
        save_deposit_receipt.addBindValue(key);
        save_deposit_receipt.exec();

        //print deposit receipt
        //QString account_number = get_acc_number(typed_cpf);  //String because it's easier to print in the receipt
        QString file_name = QFileDialog::getSaveFileName((QWidget* )0, "Export PDF", QString(), "*.pdf");
           if (QFileInfo(file_name).suffix().isEmpty()) { file_name.append(".pdf"); }

        QPrinter printer(QPrinter::PrinterResolution);
        printer.setOutputFormat(QPrinter::PdfFormat);
        printer.setPaperSize(QPrinter::A6);
        printer.setOutputFileName(file_name);

        QTextDocument receipt;
        receipt.setHtml("<pre><h1>   Gomes Bank System</h1>                Deposit receipt<br>"
                        "<br>- - - - - - - - - - - - - - - - - - - - - -<br>"
                        "FROM"
                        "<p>Name:  "+ui->depositor_name_input->text()+"</p>"
                        "<p>CPF:   "+ui->depositor_cpf_input->text()+"</p>"
                        "- - - - - - - - - - - - - - - - - - - - - -<br>"
                        "TO"
                        "<p>Name:    "+ui->name_output->text()+"</p>"
                        "<p>CPF:     "+ui->cpf_output->text()+"</p>"
                        "<p>Account:  "+ui->account_output->text()+"</p>"
                        "<p>Amount:  R$ "+ui->value_input->text()+"</p>"
                        "<p>Date:    "+deposit_time+"</p>"
                        "<p>Responsible Staff:  "+connected_staff+"</p>"
                        "<p>Deposit receipt Key:  "+key+"</p>"
                        "<pre>");
        receipt.setPageSize(printer.pageRect().size()); //hide the page number
        receipt.print(&printer);
        //<pre> preserve the text, now when type tab or space bar the text will contain it

        //clean all information in the fields
        deposit::on_depositor_cpf_input_textChanged();
        ui->depositor_name_input->setText("");
        ui->depositor_cpf_input->setText("");
        ui->favoured_account_input->setText("");
        ui->value_input->setText("");

    }
    else { ui->error_message->setText("You're not connected"); }
    close_connection_database();
}

//clean destination info when change the account
void deposit::on_favoured_account_input_textChanged() {
    ui->name_output->setText("");
    ui->cpf_output->setText("");
    ui->account_output->setText("");
    ui->error_message->setText("");
}

//erase messsage error when not all information are typed
void deposit::on_depositor_cpf_input_textChanged() {
    ui->error_message->setText("");
    deposit::on_favoured_account_input_textChanged();
}

void deposit::on_depositor_name_input_textChanged() { ui->error_message->setText(""); }

void deposit::on_value_input_textChanged()          { ui->error_message->setText(""); }
