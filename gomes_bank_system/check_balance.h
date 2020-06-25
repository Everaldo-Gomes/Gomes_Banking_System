#ifndef CHECK_BALANCE_H
#define CHECK_BALANCE_H

#include <QWidget>
#include <QMessageBox>
#include <QDebug>
#include "database_connection.h"
#include "global_variables.h"

namespace Ui {
class check_balance;
}

class check_balance : public QWidget
{
    Q_OBJECT

public:
    explicit check_balance(QWidget *parent = nullptr);
    ~check_balance();

private slots:
    void on_search_button_clicked();

    void on_close_button_clicked();

    void on_cpf_field_input_textChanged();

private:
    Ui::check_balance *ui;
};

#endif // CHECK_BALANCE_H
