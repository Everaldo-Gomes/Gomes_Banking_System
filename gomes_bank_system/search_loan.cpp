#include "search_loan.h"
#include "ui_search_loan.h"

search_loan::search_loan(QWidget *parent) : QWidget(parent), ui(new Ui::search_loan) {
    ui->setupUi(this);
    ui->cpf_field_input->setValidator(new QDoubleValidator(0, 999999999999, 2, this)); //allow only numbers. need to be double becuase the range is too long
    ui->error_message->setStyleSheet("QLabel{ color: red;}"); //error message color
    ui->cpf_field_input->setFocus();
}

search_loan::~search_loan() { delete ui; }

//close button
void search_loan::on_close_button_clicked() { this->close(); }

//clean all fields when change cpf
void search_loan::on_cpf_field_input_textChanged() {
    ui->error_message->setText("");
    ui->name_output->setText("");
    ui->cpf_output->setText("");
    ui->account_output->setText("");
    ui->installments_output->setText("");
    ui->fees_output->setText("");
    ui->amount_month_output->setText("");
    ui->amount_output->setText("");
    ui->installment_paid_output->setText("");
    ui->payday_output->setText("");
    ui->loan_quantity_output->setText("");
    ui->loan_list_box->clear();

    //need to clean all table as well
}

//search button
void search_loan::on_search_button_clicked() {
    ui->loan_list_box->clear();
    QString typed_cpf = ui->cpf_field_input->text();

    if(typed_cpf == "") { ui->error_message->setText("Enter a valid CPF."); }
    else if(connect_database()) {
        if(!search_customer_cpf(typed_cpf)) {

            int count = 0;

            //get loans receipt
            QSqlQuery get_loans;
            get_loans.exec("SELECT lr.loan_receipt_key, ct.full_name, lr.loan_date, lr.fees FROM loan_receipt lr JOIN customer ct ON ct.id = lr.customer_id WHERE ct.cpf = '"+typed_cpf+"';");
            while(get_loans.next()) {
                count++;
                ui->loan_list_box->addItem(get_loans.value(0).toString());
                ui->name_output->setText(get_loans.value(1).toString());
            }
            ui->cpf_output->setText(typed_cpf);
            ui->loan_quantity_output->setText(QString::number(count));
        }
        else { ui->error_message->setText("CPF is not registerd"); }
        close_connection_database();
    }
    else { ui->error_message->setText("You re not connected"); }
}

//get all information when a loan receipt key is selected
void search_loan::on_loan_list_box_currentIndexChanged() {
    ui->installments_output->setText("");
    ui->fees_output->setText("");
    ui->amount_month_output->setText("");
    ui->amount_output->setText("");
    ui->installment_paid_output->setText("");
    ui->payday_output->setText("");


    if(connect_database()) {

        //get specific information about each loan
        QString get_selected_key = ui->loan_list_box->currentText();

        QSqlQuery get_info;    //DELETE LR.LOAN_DATE
        get_info.exec("SELECT lr.fees, lr.payment_installments, lr.amount, lr.amount_per_month, lr.payday FROM loan_receipt lr JOIN customer ct ON ct.id = lr.customer_id WHERE lr.loan_receipt_key = '"+get_selected_key+"';");

        while(get_info.next()) {

            double fees = get_info.value(0).toDouble();
            ui->fees_output->setText(QString::number(fees,'f',2) + " %");

            ui->installments_output->setText(get_info.value(1).toString());

            QString has_it = "";
            has_it = loan_paid_any(get_selected_key);
            if(has_it == "") { ui->installment_paid_output->setText("0"); }
            else { ui->installment_paid_output->setText(has_it); }

            double amount = get_info.value(2).toDouble();
            ui->amount_output->setText("R$ " + QString::number(amount,'f',2));

            double amount_month = get_info.value(3).toDouble();
            ui->amount_month_output->setText("R$ " + QString::number(amount_month,'f',2));

            ui->payday_output->setText(get_info.value(4).toString());
        }
        ui->account_output->setText(get_acc_number(ui->cpf_field_input->text()));

        //other information on the big white filed "table"
        QSqlQuery get_info2;
        get_info2.exec("SELECT payer_name as Name, payer_cpf as CPF, loan_receipt_key as receipt_key, payment_installment_paid as Qnt_Paid, payment_day as Date, on_time  FROM loan_payment_control WHERE  loan_receipt_key = '"+get_selected_key+"';");

        QSqlQueryModel *model = new QSqlQueryModel();
        model->setQuery(get_info2);

        ui->tableView->setModel(model);

        //ui->tableView->resizeColumnsToContents(); //resize automatically
        //set a  predefine value (size) to each column, in this case, it's better
        ui->tableView->setColumnWidth(0,250);
        ui->tableView->setColumnWidth(1,150);
        ui->tableView->setColumnWidth(2,150);
        ui->tableView->setColumnWidth(3,100);
        ui->tableView->setColumnWidth(4,150);
        ui->tableView->setColumnWidth(5,50);
    }
    else { ui->error_message->setText("You re not connected"); }
}
