#include "pay_loan.h"
#include "ui_pay_loan.h"

pay_loan::pay_loan(QWidget *parent) : QWidget(parent), ui(new Ui::pay_loan) {
    ui->setupUi(this);
    ui->payer_cpf_input->setValidator(new QDoubleValidator(0, 999999999999, 2, this));
    ui->pay_button->setDisabled(true);
    ui->error_message->setStyleSheet("QLabel{ color: red;}");
}

pay_loan::~pay_loan() { delete ui; }

//clear all info when change receipt key
void pay_loan::on_receipt_key_input_textChanged() {
    ui->installments_output->setText("");
    ui->current_installment_output->setText("");
    ui->fees_output->setText("");
    ui->fees_increase_output->setText("");
    ui->on_time_output->setText("");
    ui->total_fees_output->setText("");
    ui->original_value_output->setText("");
    ui->total_pay_output->setText("");
    ui->error_message->setText("");
    ui->pay_button->setDisabled(true);
}

//cancel button
void pay_loan::on_cancel_button_clicked() { this->close(); }

//check button
void pay_loan::on_check_button_clicked() {

    QString key = ui->receipt_key_input->text();
    QString found_payment_installments;
    int     found_payday;
    double found_fees;
    double found_amout_per_month;
    double final_value;
    QString found_key = "";

    if(connect_database()) {
          QSqlQuery search_key;
          search_key.exec("SELECT * FROM loan_receipt WHERE loan_receipt_key = '"+key+"';");
          while(search_key.next()) {
              found_payment_installments = search_key.value(4).toString();
              found_fees = search_key.value(5).toDouble();
              found_amout_per_month = search_key.value(6).toDouble();
              final_value = found_amout_per_month;
              found_payday = search_key.value(7).toInt();
              found_key = search_key.value(10).toString();
          }
          if(found_key != "") { //the loan was found
              ui->installments_output->setText(found_payment_installments);
              ui->fees_output->setText(QString::number(found_fees,'f',2) + " %");

              int current_day = QDate::currentDate().toString("dd").toInt();

              //if it is delayed increase the fees by day % for each day
              if(current_day > found_payday) {
                  ui->on_time_output->setStyleSheet("QLabel{ color: red;}");
                  ui->on_time_output->setText("NO");

                  double percent_increased_day = (current_day - found_payday);
                  percent_increased_day /= 100;

                  double total_increased = percent_increased_day;
                  ui->fees_increase_output->setText(QString::number(total_increased,'f',2) + " %");
                  found_fees += percent_increased_day;

                  if(total_increased > 0.00) { final_value = found_amout_per_month + (found_amout_per_month * found_fees); }
                  else { final_value = found_amout_per_month; }
              }
              else {
                  ui->on_time_output->setStyleSheet("QLabel{ color: green;}");
                  ui->on_time_output->setText("YES");
                  ui->fees_increase_output->setText("0.00 %");
              }
              ui->total_fees_output->setText(QString::number(found_fees,'f',2) + " %");
              ui->original_value_output->setText("R$ " + QString::number(found_amout_per_month,'f',2));
              ui->total_pay_output->setText("R$ " + QString::number(final_value,'f',2));

              //check if all installment were paid
              QString paying = loan_paid_any(ui->receipt_key_input->text());
              QString installments = ui->installments_output->text();
              if(installments == paying) { ui->current_installment_output->setText("Done"); }
              else {
                  //show which installment is being paid
                  QString paid = loan_paid_any(found_key);
                  if(paid == "") { ui->current_installment_output->setText("1"); }
                  else {
                      //get the qnt the database and add 1
                      int p = paid.toInt();
                      paid = QString::number(p+1);
                      ui->current_installment_output->setText(paid);
                  }
              }
              ui->pay_button->setDisabled(false);
          }
          else { ui->error_message->setText("Loan not found"); }
          close_connection_database();
    }
    else { ui->error_message->setText("You re not connected"); }
}

//pay loan button
void pay_loan::on_pay_button_clicked() {

    if(ui->payer_name_input->text() == "" || ui->payer_cpf_input->text() == "") { ui->error_message->setText("Enter all information"); }
    else {

        //ask for a confirmation
        QMessageBox::StandardButton confirmation;
        confirmation = QMessageBox::question(this, "Confirm", "Are you sure?", QMessageBox::Yes | QMessageBox::No);

        if(confirmation == QMessageBox::No) { /*do nothing*/ }
        else if(connect_database()) {

            //check if all installment were paid
            QString paying = loan_paid_any(ui->receipt_key_input->text());
            QString installments = ui->installments_output->text();

            if(installments == paying) { ui->error_message->setText("All installments were paid"); }
            else {

                //message payment done
                QMessageBox::information(this,"Payment", "Payment has been done.\nPress Ok to print the loan payment receipt.");

                //save into loan payment control
                QSqlQuery control;
                control.prepare("INSERT INTO loan_payment_control (payer_name, payer_cpf, loan_receipt_key, payment_installment_paid, payment_day, on_time)"
                "VALUES(?,?,?,?,?,?);");
                control.addBindValue(ui->payer_name_input->text());
                control.addBindValue(ui->payer_cpf_input->text());
                control.addBindValue(ui->receipt_key_input->text());

                //get the qnt the database and add 1
                QString qnt = loan_paid_any(ui->receipt_key_input->text());
                if(qnt == "") { control.addBindValue("1"); }  //if the first installment is being paid
                else { control.addBindValue(qnt.toInt()+1); }

                control.addBindValue(QDateTime::currentDateTime());
                control.addBindValue(ui->on_time_output->text());
                control.exec();

                //QString payment_day = QDateTime::currentDateTime().toString();

                //print payment receipt
                QString wirthdrawal_time = QDateTime::currentDateTime().toString();
                QString file_name = QFileDialog::getSaveFileName((QWidget* )0, "Export PDF", QString(), "*.pdf");
                if(QFileInfo(file_name).suffix().isEmpty()) { file_name.append(".pdf"); }

                QPrinter printer(QPrinter::PrinterResolution);
                printer.setOutputFormat(QPrinter::PdfFormat);
                printer.setPaperSize(QPrinter::A6);
                printer.setOutputFileName(file_name);

                QTextDocument receipt;
                receipt.setHtml("<pre><h1>   Gomes Bank System</h1>            Loan payment receipt<br>"
                                "<p>Name:  "+ui->payer_name_input->text()+"</p>"
                                "<p>CPF:   "+ui->payer_cpf_input->text()+"</p>"
                                "<p>Amount:  R$ "+ui->total_pay_output->text()+"</p>"
                                "<p>Payment stallement: "+ui->installments_output->text()+"x</p>"
                                "<p>Stallement paid: "+ui->current_installment_output->text()+"º</p>"
                                "<p>Payment day:  "+QDateTime::currentDateTime().toString()+"</p>"
                                "<p>Responsible Staff:  "+connected_staff+"</p>"
                                "<p>Loan receipt Key:  "+ui->receipt_key_input->text()+"</p>"
                                "<pre>");
                receipt.setPageSize(printer.pageRect().size()); //hide the page number
                receipt.print(&printer);
                //<pre> preserve the text, now when type tab or space bar the text will contain it

                //clean all fields
                pay_loan::on_receipt_key_input_textChanged();
                ui->payer_name_input->setText("");
                ui->payer_cpf_input->setText("");
                ui->receipt_key_input->setText("");
            }
        }
        else { ui->error_message->setText("You re not connected"); }
    }
}

//erase message whe change name & cpf
void pay_loan::on_payer_name_input_textChanged() { ui->error_message->setText(""); }

void pay_loan::on_payer_cpf_input_textChanged() { ui->error_message->setText(""); }
