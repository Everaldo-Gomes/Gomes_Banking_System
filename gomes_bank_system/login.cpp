#include "login.h"
#include "ui_login.h"

QString connected_id;
QString connected_staff;
QString connected_sector;

Login::Login(QWidget *parent) : QMainWindow(parent), ui(new Ui::Login) {
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);  //remove tittle bar || making a frameless window
    ui->cpf_field->setValidator(new QDoubleValidator(0, 999999999999, 2, this)); //allow only numbers. need to be double becuase the range is too long
}

Login::~Login() {
    delete ui;
}

//erase the error message (if any) when changing texxt on either cpf or password field
void Login::on_cpf_field_textEdited() {
    ui->error_message->setText("");
}

void Login::on_password_field_textEdited() {
    ui->error_message->setText("");
}

//cancel button
void Login::on_cancel_button_clicked() {
 this->close();
}

//login button
void Login::on_login_button_clicked() {

    //get what was typed in cpf & password field
    QString typed_cpf = ui->cpf_field->text();
    QString typed_pw  = ui->password_field->text();

    //just connect to the database only if both information were typed
    if(typed_cpf == "" && typed_pw != "")      { ui->error_message->setText("Enter a CPF"); }
    else if(typed_cpf != "" && typed_pw == "") { ui->error_message->setText("Enter a password"); }
    else if(typed_cpf == "" && typed_pw == "") { ui->error_message->setText("Enter a CPF and a password"); }
    else {
        //connect to the database to check information
        QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL");

        db.setHostName("localhost");
        db.setDatabaseName("gomes_bank_system");
        db.setUserName("main");
        db.setPassword("gomes");  //actual passord
        db.open();

        if(db.open()) {
            bool not_found = true;

            //check cpf and password
            QSqlQuery check_login;
            check_login.exec("SELECT * FROM staff;");

            while(check_login.next()) {
                QString found_id     = check_login.value(0).toString();
                QString found_name   = check_login.value(1).toString();
                QString found_cpf    = check_login.value(2).toString();
                QString found_sector = check_login.value(7).toString();
                QString found_pw     = check_login.value(8).toString();

                connected_id     = found_id;
                connected_staff  = found_name;
                connected_sector = found_sector;

                if(found_cpf == typed_cpf && found_pw == typed_pw) {
                    //call main screen
                    main_screen *obj_main_screen = new main_screen();
                    obj_main_screen->show();

                    not_found = false; //if CPF and password were found does not show a error message
                    this->close();  //close login screnn
                    break; //stop searching into the database
                }
            }
            //if CPF or password are invalid
            if(not_found) { ui->error_message->setText("Invalid informatioin"); }
        }
        else { ui->error_message->setText("You're not connected"); }
        db.close();
    }
}
