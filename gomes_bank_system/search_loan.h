#ifndef SEARCH_LOAN_H
#define SEARCH_LOAN_H

#include <QWidget>
#include <QIntValidator>
#include <QMessageBox>
#include <QDebug>
#include "database_connection.h"
#include "global_variables.h"

namespace Ui {
class search_loan;
}

class search_loan : public QWidget
{
    Q_OBJECT

public:
    explicit search_loan(QWidget *parent = nullptr);
    ~search_loan();

private slots:
    void on_search_button_clicked();

    void on_cpf_field_input_textChanged();

    void on_close_button_clicked();

    void on_loan_list_box_currentIndexChanged();

private:
    Ui::search_loan *ui;
};

#endif // SEARCH_LOAN_H
