#ifndef CHANGE_CUSTOMER_INFO_H
#define CHANGE_CUSTOMER_INFO_H

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
class change_customer_info;
}

class change_customer_info : public QWidget
{
    Q_OBJECT

public:
    explicit change_customer_info(QWidget *parent = nullptr);
    ~change_customer_info();

private slots:
    void on_cpf_field_input_textChanged();

    void on_cancel_button_clicked();

    void on_search_button_clicked();

    void on_change_button_clicked();

    void on_reason_message_input_textChanged();

    void on_current_password_input_textChanged();

    void on_password_input_textChanged();

    void on_confirm_passowrd_input_textChanged();

private:
    Ui::change_customer_info *ui;
};

#endif // CHANGE_CUSTOMER_INFO_H
