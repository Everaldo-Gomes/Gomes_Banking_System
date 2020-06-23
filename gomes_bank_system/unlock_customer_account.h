#ifndef UNLOCK_CUSTOMER_ACCOUNT_H
#define UNLOCK_CUSTOMER_ACCOUNT_H

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

#include "database_connection.h"
#include "global_variables.h"
#include "login.h"

namespace Ui {
class unlock_customer_account;
}

class unlock_customer_account : public QWidget
{
    Q_OBJECT

public:
    explicit unlock_customer_account(QWidget *parent = nullptr);
    ~unlock_customer_account();

private slots:
    void on_search_button_clicked();

    void on_reason_message_input_textChanged();

    void on_cpf_field_input_textChanged();

    void on_cancel_button_clicked();

    void on_unlock_button_clicked();

private:
    Ui::unlock_customer_account *ui;
};

#endif // UNLOCK_CUSTOMER_ACCOUNT_H
