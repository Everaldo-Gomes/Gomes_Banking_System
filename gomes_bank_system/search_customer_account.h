#ifndef SEARCH_CUSTOMER_ACCOUNT_H
#define SEARCH_CUSTOMER_ACCOUNT_H

#include <QWidget>
#include <QIntValidator>
#include <QMessageBox>
#include <QDebug>
#include "database_connection.h"
#include "global_variables.h"

namespace Ui {
class search_customer_account;
}

class search_customer_account : public QWidget
{
    Q_OBJECT

public:
    explicit search_customer_account(QWidget *parent = nullptr);
    ~search_customer_account();

private slots:
    void on_close_button_clicked();

    void on_search_button_clicked();

    void on_cpf_field_input_textChanged();

private:
    Ui::search_customer_account *ui;
};

#endif // SEARCH_CUSTOMER_ACCOUNT_H
