#ifndef DEPOSIT_H
#define DEPOSIT_H

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
class deposit;
}

class deposit : public QWidget
{
    Q_OBJECT

public:
    explicit deposit(QWidget *parent = nullptr);
    ~deposit();

private slots:


    void on_cancel_button_clicked();

    void on_check_button_clicked();

    void on_deposit_button_clicked();

    void on_favoured_account_input_textChanged();

    void on_depositor_cpf_input_textChanged();

    void on_depositor_name_input_textChanged();

    void on_value_input_textChanged();

private:
    Ui::deposit *ui;
};

#endif // DEPOSIT_H
