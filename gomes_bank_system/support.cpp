#include "support.h"
#include "ui_support.h"

void add(int id, QString day, QString message);

support::support(QWidget *parent) : QWidget(parent), ui(new Ui::support) {
    ui->setupUi(this);
}

support::~support() {
    delete ui;
}

//text area
void support::on_support_message_textChanged() {

    int max_length = 255;

    //if reach the character limit delete the following characters
    if(ui->support_message->toPlainText().length() > max_length){
        ui->support_message->textCursor().deletePreviousChar();
    }

    //exhibit the characters limit on the screen
    int actual_len = ui->support_message->toPlainText().length();
    int len_to_show = max_length - actual_len;
    ui->character_limit->setText(QString::number(len_to_show));  //convert to string

    ui->error_message->setText("");  //clean the error message
}

//close button
void support::on_cancel_button_clicked() {
    this->close();
}

//send button
void support::on_send_button_clicked() {

    QString report_message = ui->support_message->toPlainText(); //get what was typed

    if(report_message == "") { ui->error_message->setText("Type some message"); }
    else {

        //ask for a confimation before sending the message
        QMessageBox::StandardButton confirmation;
        confirmation = QMessageBox::question(this, "Confirm", "Are you sure?",
                                      QMessageBox::Yes | QMessageBox::No);

        if(confirmation == QMessageBox::No) { /*do nothing*/ }
        else if(connect_database()) {              //store information
                QSqlQuery insert_message_query;
                insert_message_query.prepare("INSERT INTO support (staff_id, opening_day, message)"
                                             "VALUES (?,?,?)");

                insert_message_query.addBindValue(connected_id.toInt());
                insert_message_query.addBindValue(QDateTime::currentDateTime());  //show all time'
                insert_message_query.addBindValue(report_message);
                insert_message_query.exec();

                //show a message that the content has been sent
                QMessageBox::information(this,"About your message", "Your meessage has been sent");
                ui->support_message->setText("");
            }
        else { ui->error_message->setText("You're not connected"); }
        close_connection_database();
   }
}

/*
QString current_date = QDate::currentDate().toString("yyyy.MM.dd");
QString current_time = QTime::currentTime().toString("hh.mm.ss");
insert_message_query.addBindValue(QDateTime::fromString(current_date,"yyyy-mm-dd"));  use only year-month-day
*/
