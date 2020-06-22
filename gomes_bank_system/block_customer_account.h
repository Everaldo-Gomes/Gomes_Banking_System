#ifndef BLOCK_CUSTOMER_ACCOUNT_H
#define BLOCK_CUSTOMER_ACCOUNT_H

#include <QWidget>
#include <QMessageBox>
#include <QDebug>
#include "database_connection.h"
#include "global_variables.h"

namespace Ui {
class block_customer_account;
}

class block_customer_account : public QWidget
{
    Q_OBJECT

public:
    explicit block_customer_account(QWidget *parent = nullptr);
    ~block_customer_account();

private slots:
    void on_cancel_button_clicked();

    void on_search_button_clicked();

    void on_block_button_clicked();

    void on_reason_message_input_textChanged();

    void on_cpf_field_input_textChanged();

private:
    Ui::block_customer_account *ui;
};

#endif // BLOCK_CUSTOMER_ACCOUNT_H
