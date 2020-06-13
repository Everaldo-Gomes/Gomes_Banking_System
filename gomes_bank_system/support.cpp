#include "support.h"
#include "ui_support.h"
#include <QDateTime>

void add(int id, QString day, QString message);

support::support(QWidget *parent) : QWidget(parent), ui(new Ui::support) {
    ui->setupUi(this);
}

support::~support() {
    delete ui;
}

//text area
void support::on_support_message_textChanged() {

    int max_length = 500;

    //if reach the character limit delete the following letters
    if(ui->support_message->toPlainText().length() > max_length){
        ui->support_message->textCursor().deletePreviousChar();
    }

    //exhibit the characters limit on the screen
    int actual_len = ui->support_message->toPlainText().length();
    int len_to_show = max_length - actual_len;
    ui->character_limit->setText(QString::number(len_to_show));  //convert to string
}

//close button
void support::on_cancel_button_clicked() {
    this->close();
}

//send button
void support::on_send_button_clicked() {

    QString report_message = ui->support_message->toPlainText();

    if(report_message == "") { ui->error_message->setText("Type some message"); }
    else {
        //connect to the database to check information
        QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL");

        db.setHostName("localhost");
        db.setDatabaseName("gomes_bank_system");
        db.setUserName("main");
        db.setPassword("gomes");  //actual passord
        db.open();

        QSqlQuery insert_message;
        int var = connected_id.toInt();

        if(db.open()) {
           insert_message.exec("insert into support (staff_id, opening_day, message) values (f,'2020-11-11', '"+report_message+"');");
        }
        else { ui->error_message->setText("You're not connected"); }
        db.close();
    }
}
