#ifndef ADD_CUSTOMER_ACCOUNT_H
#define ADD_CUSTOMER_ACCOUNT_H

#include <QWidget>
#include <QCoreApplication>
#include <QSqlDatabase>
#include <QtSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QIntValidator>
#include <QDateTime>
#include <QComboBox>
#include <QMessageBox>
#include <QDebug>
#include <QLocale>
#include <QtPrintSupport/QPrinter>
#include <QtPrintSupport/QPrintDialog>
#include "QPainter"
#include <QPdfWriter>
#include <QTextEdit>
#include <QFile>
#include <QFileInfo>
#include <QFileDialog>
#include <QTextStream>


#include "database_connection.h"
#include "global_variables.h"
#include "login.h"

namespace Ui {
class add_customer_account;
}

class add_customer_account : public QWidget
{
    Q_OBJECT

public:
    explicit add_customer_account(QWidget *parent = nullptr);
    ~add_customer_account();

private slots:
    void on_add_button_clicked();

    void on_name_input_textChanged();

    void on_cpf_input_textChanged();

    void on_phone_input_textChanged();

    void on_address_input_textChanged();

    void on_email_input_textChanged();

    void on_account_password_input_textChanged();

    void on_account_confirm_passowrd_input_textChanged();

    void on_amount_input_textChanged();

    void on_cancel_button_clicked();

private:
    Ui::add_customer_account *ui;
};

#endif // ADD_CUSTOMER_ACCOUNT_H
