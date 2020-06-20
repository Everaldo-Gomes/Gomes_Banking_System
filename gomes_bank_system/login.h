#ifndef LOGIN_H
#define LOGIN_H

#include <QMainWindow>

#include <QCoreApplication>
#include <QSqlDatabase>
#include <QtSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QIntValidator>
#include <QDebug>

#include "main_screen.h"
#include "ui_main_screen.h"
#include <global_variables.h>

QT_BEGIN_NAMESPACE
namespace Ui { class Login; }
QT_END_NAMESPACE

class Login : public QMainWindow
{
    Q_OBJECT

public:
    Login(QWidget *parent = nullptr);
    ~Login();

    QString encrypt_password(QString s) {
        QByteArray pw = s.toUtf8();                 //convert string to QByteArray
        QByteArray encrypt = pw;
        QCryptographicHash hasher(QCryptographicHash::Sha1);
        hasher.addData(encrypt);
        QByteArray new_password = hasher.result();
        QString encrypted_password = new_password;   //convert qByteArray to string
        return encrypted_password;
    }

private slots:

    void on_cpf_field_textEdited();

    void on_password_field_textEdited();

    void on_login_button_clicked();

    void on_cancel_button_clicked();

private:
    Ui::Login *ui;
};
#endif // LOGIN_H
