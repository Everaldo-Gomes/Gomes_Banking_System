#ifndef PAY_LOAN_H
#define PAY_LOAN_H

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

namespace Ui {
class pay_loan;
}

class pay_loan : public QWidget
{
    Q_OBJECT

public:
    explicit pay_loan(QWidget *parent = nullptr);
    ~pay_loan();

private slots:
    void on_cancel_button_clicked();

    void on_check_button_clicked();

    void on_pay_button_clicked();

    void on_receipt_key_input_textChanged();

    void on_payer_name_input_textChanged();

    void on_payer_cpf_input_textChanged();

private:
    Ui::pay_loan *ui;
};

#endif // PAY_LOAN_H
