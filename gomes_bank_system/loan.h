#ifndef LOAN_H
#define LOAN_H

#include <QWidget>
#include <QMessageBox>
#include <QDebug>
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
class loan;
}

class loan : public QWidget
{
    Q_OBJECT

public:
    explicit loan(QWidget *parent = nullptr);
    ~loan();

private slots:
    void on_cancel_button_clicked();

    void on_cpf_field_input_textChanged();

    void on_search_button_clicked();

    void on_loan_button_clicked();

    void on_check_button_clicked();

    void on_resquested_amount_input_textChanged();

    void on_payment_installments_input_textChanged();

    void on_payday_intput_textChanged();

    void on_password_input_textChanged();

private:
    Ui::loan *ui;
};

#endif // LOAN_H
