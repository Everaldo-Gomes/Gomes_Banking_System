#ifndef ADD_STAFF_H
#define ADD_STAFF_H

#include <QWidget>
#include <QCoreApplication>
#include <QSqlDatabase>
#include <QtSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QIntValidator>
#include <QDateTime>
#include <QComboBox>
#include <QMessageBox>
#include <QDebug>
#include "global_variables.h"

namespace Ui {
class add_staff;
}

class add_staff : public QWidget
{
    Q_OBJECT

public:
    explicit add_staff(QWidget *parent = nullptr);
    ~add_staff();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void on_name_input_textChanged();

    void on_cpf_input_textChanged();

    void on_phone_input_textChanged();

    void on_address_input_textChanged();

    void on_email_input_textChanged();

    void on_password_input_textChanged();

    void on_confirm_passowrd_input_textChanged();

    void on_sector_input_currentTextChanged();

    void on_day_input_valueChanged();

    void on_month_input_valueChanged();

    void on_year_input_valueChanged();

private:
    Ui::add_staff *ui;
};

#endif // ADD_STAFF_H
