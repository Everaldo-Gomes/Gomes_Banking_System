#include "loan.h"
#include "ui_loan.h"

int custo_id;
QString typed_cpf, payment_installments, fees, payday, requested_amount;
double final_value;

loan::loan(QWidget *parent) : QWidget(parent), ui(new Ui::loan) {
    ui->setupUi(this);
    ui->cpf_field_input->setValidator(new QDoubleValidator(0, 999999999999, 2, this));
    ui->resquested_amount_input->setValidator(new QDoubleValidator(0, 999999999999, 2, this));
    ui->payment_installments_input->setValidator(new QDoubleValidator(0, 999999999999, 2, this));
    ui->payday_intput->setValidator(new QDoubleValidator(0, 999999999999, 2, this));
    ui->loan_button->setDisabled(true); //disable loan button
    ui->resquested_amount_input->setDisabled(true);
    ui->payment_installments_input->setDisabled(true);
    ui->payday_intput->setDisabled(true);
    ui->password_input->setDisabled(true);
    ui->check_button->setDisabled(true);
    ui->error_message->setStyleSheet("QLabel{ color: red;}"); //error message color
    ui->error_message_cpf->setStyleSheet("QLabel{ color: red;}"); //error message color
    ui->cpf_field_input->setFocus();
}

loan::~loan() { delete ui; }

//cancel button
void loan::on_cancel_button_clicked() { this->close(); }

//clean info when change cpf
void loan::on_cpf_field_input_textChanged() {
    ui->name_output->setText("");
    ui->cpf_output->setText("");
    ui->account_output->setText("");
    ui->error_message->setText("");
    ui->error_message_cpf->setText("");
    ui->resquested_amount_input->setDisabled(true);
    ui->payment_installments_input->setDisabled(true);
    ui->payday_intput->setDisabled(true);
    ui->password_input->setDisabled(true);
    ui->check_button->setDisabled(true);
    ui->loan_button->setDisabled(true);
}

//search button
void loan::on_search_button_clicked() {
    QString typed_cpf = ui->cpf_field_input->text();

    if(typed_cpf == "") { ui->error_message_cpf->setText("Enter a valid CPF."); }
    else if(connect_database()) {
        if(!search_customer_cpf(typed_cpf)) {

            //show information
            QSqlQuery get_info;
            get_info.exec("SELECT * FROM customer");

            while(get_info.next()) {
                QString found_id    = get_info.value(0).toString();
                QString found_name  = get_info.value(1).toString();
                QString found_cpf   = get_info.value(2).toString();

                custo_id = found_id.toInt(); //used when click on block button

                if(typed_cpf == found_cpf) {
                    ui->name_output->setText(found_name);
                    ui->cpf_output->setText(found_cpf);
                    break;
                }
            }
            ui->account_output->setText(get_acc_number(typed_cpf)); //show account number

            //enable changeable fields and check button
            ui->resquested_amount_input->setDisabled(false);
            ui->payment_installments_input->setDisabled(false);
            ui->payday_intput->setDisabled(false);
            ui->password_input->setDisabled(false);
            ui->check_button->setDisabled(false);
        }
        else { ui->error_message_cpf->setText("CPF is not registerd"); }
    }
    else { ui->error_message_cpf->setText("You re not connected"); }
}

//check button
void loan::on_check_button_clicked() {
    typed_cpf          = ui->cpf_field_input->text();
    requested_amount   = ui->resquested_amount_input->text();
    payment_installments = ui->payment_installments_input->text();
    payday             = ui->payday_intput->text();
    fees              = ui->fees_output->text();
    double fees, fees_per_month = 1.1; //define the fees % a.m

    if(requested_amount == "" || payment_installments == "" || payday == "") {
        ui->error_message->setText("Enter all needed information");
        ui->loan_button->setDisabled(true);
    }
    //if the customer/account is blocked denied the operation
    else if(customer_blocked(typed_cpf)) { ui->error_message->setText("Customer/account blocked"); }

    //verify the values
    else if(requested_amount.toDouble() < 1000) { ui->error_message->setText("The minimum value is 1000.00"); }
    else if(payment_installments.toInt() <= 0 || payday.toInt() <= 0 ) {
        ui->error_message->setText("You cannot enter 0");
        ui->loan_button->setDisabled(true);
    }
    //verify if the value is right
    else if(!has_decimal_point(ui->resquested_amount_input->text())) { ui->error_message->setText("The value need to have decimal point. Ex: 50.00"); }
    else if(connect_database()) {

        //if the customer wasnt blocked yet the fees will still the same
        if(blocked_customer_acc_many_times(custo_id)) {
            fees = fees_per_month * payment_installments.toInt();
            ui->fees_output->setText(QString::number(fees,'f',2)); }  //f,2 mean that will show a float number with 2 numbers after decimal point

         //if the customer was blocked the fee will have some changes
        else {
            fees = 1.3 * fees_per_month;
            ui->fees_output->setText(QString::number(fees,'f',2));
        }

        //amount per month
        final_value = (requested_amount.toDouble() / payment_installments.toInt()) * fees;
        ui->amount_output->setText(QString::number(final_value,'f',2));

        ui->loan_button->setDisabled(false);
    }
    else { ui->error_message->setText("You re not connected"); }
}

//loan button
void loan::on_loan_button_clicked() {

    //verify password
    Login *l = new Login();
    QString encrypted_password = l->encrypt_password(ui->password_input->text());
    QString found_pw;

    QSqlQuery get_pw;
    get_pw.prepare("SELECT password FROM account WHERE customer_id = ?");
    get_pw.addBindValue(custo_id);
    get_pw.exec();
    while(get_pw.next()) { found_pw = get_pw.value(0).toString(); }

    if(found_pw == encrypted_password) {
        //ask for a confirmation
        QMessageBox::StandardButton confirmation;
        confirmation = QMessageBox::question(this, "Confirm", "Are you sure?", QMessageBox::Yes | QMessageBox::No);

        if(confirmation == QMessageBox::No) { /*do nothing*/ }
        else {
            //add amount in the account
            QString amount = get_account_amount(custo_id); //get the current amount
            double current_value = amount.toDouble();
            double requested_amount = ui->resquested_amount_input->text().toDouble();
            add_amount(requested_amount + current_value, typed_cpf); //add the value in the account

            //save loan receipt
            QSqlQuery save_loan_receipt;
            save_loan_receipt.prepare("INSERT INTO loan_receipt (customer_id, account_id, amount, payment_installments, fees, amount_per_month, payday, loan_date, responsible_staff_id, loan_receipt_key)"
                                      "VALUES(?,?,?,?,?,?,?,?,?,?);");
            save_loan_receipt.addBindValue(custo_id);
            save_loan_receipt.addBindValue(get_acc_id(custo_id));
            save_loan_receipt.addBindValue(requested_amount);
            save_loan_receipt.addBindValue(ui->payment_installments_input->text().toInt());
            save_loan_receipt.addBindValue(fees.toDouble());
            save_loan_receipt.addBindValue(final_value);
            save_loan_receipt.addBindValue(payday);

            QString loan_time = QDateTime::currentDateTime().toString();
            save_loan_receipt.addBindValue(loan_time);
            save_loan_receipt.addBindValue(connected_id);

            //generate receipt key   AC =account, LO = loan
            int last_id;
            QSqlQuery get_last_id;
            get_last_id.exec("SELECT id FROM loan_receipt ORDER BY id DESC LIMIT 1;"); //get the last id
            while(get_last_id.next()) { last_id = get_last_id.value(0).toInt(); }

            last_id *= 7;
            QString key = "AC" + QString::number(last_id) + "LO";
            save_loan_receipt.addBindValue(key);
            save_loan_receipt.exec();

            //massage action done
            QMessageBox::information(this,"Loan", "Loan has been done.\nPress Ok to print the loan receipt.");

            //on loan receipt, show values properly
            double aux = ui->amount_output->text().toDouble();
            QString amount_out = QString::number(aux,'f',2);

            //print loan receipt
            QString wirthdrawal_time = QDateTime::currentDateTime().toString();
            QString file_name = QFileDialog::getSaveFileName((QWidget* )0, "Export PDF", QString(), "*.pdf");
            if(QFileInfo(file_name).suffix().isEmpty()) { file_name.append(".pdf"); }

            QPrinter printer(QPrinter::PrinterResolution);
            printer.setOutputFormat(QPrinter::PdfFormat);
            printer.setPaperSize(QPrinter::A6);
            printer.setOutputFileName(file_name);

            QTextDocument receipt;
            receipt.setHtml("<pre><h1>   Gomes Bank System</h1>                Loan receipt<br>"
                            "<p>Name:  "+ui->name_output->text()+"</p>"
                            "<p>CPF:   "+ui->cpf_output->text()+"</p>"
                            "<p>Account:  "+ui->account_output->text()+"</p>"
                            "<p>Amount:  R$ "+ui->resquested_amount_input->text()+"</p>"
                            "<p>fees: "+ui->fees_output->text()+" % a.m </p>"
                            "<p>Amount per month: R$ "+amount_out+"</p>"
                            "<p>Payment stallement: "+ui->payment_installments_input->text()+"x</p>"
                            "<p>Payment day: "+ui->payday_intput->text()+"</p>"
                            "<p>Date:  "+loan_time+"</p>"
                            "<p>Responsible Staff:  "+connected_staff+"</p>"
                            "<p>Loan receipt Key:  "+key+"</p>"
                            "<pre>");
            receipt.setPageSize(printer.pageRect().size()); //hide the page number
            receipt.print(&printer);
            //<pre> preserve the text, now when type tab or space bar the text will contain it

            //clean all fields
            loan::on_cpf_field_input_textChanged();
            ui->cpf_field_input->setText("");
            ui->resquested_amount_input->setText("");
            ui->payment_installments_input->setText("");
            ui->payday_intput->setText("");
            ui->amount_output->setText("");
            ui->fees_output->setText("");
            ui->password_input->setText("");
        }
    }
    else { ui->error_message->setText("Password does not match"); }
}

//clean message error when type
void loan::on_resquested_amount_input_textChanged()  {
    ui->error_message->setText("");
    ui->loan_button->setDisabled(true);
}

void loan::on_payment_installments_input_textChanged() {
    ui->error_message->setText("");
    ui->loan_button->setDisabled(true);
}

void loan::on_payday_intput_textChanged() {
    ui->error_message->setText("");
    ui->loan_button->setDisabled(true);
}

void loan::on_password_input_textChanged() { ui->error_message->setText(""); }


