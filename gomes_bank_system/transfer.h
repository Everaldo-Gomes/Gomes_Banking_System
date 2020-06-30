#ifndef TRANSFER_H
#define TRANSFER_H

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
class transfer;
}

class transfer : public QWidget
{
    Q_OBJECT

public:
    explicit transfer(QWidget *parent = nullptr);
    ~transfer();

private slots:
    void on_close_button_clicked();

    void on_transfer_button_clicked();

    void on_name_from_input_textChanged();

    void on_cpf_from_input_textChanged();

    void on_account_from_input_textChanged();

    void on_value_from_input_textChanged();

    void on_password_input_textChanged();

    void on_name_to_input_textChanged();

    void on_cpf_to_input_textChanged();

    void on_account_to_input_textChanged();

private:
    Ui::transfer *ui;
};

#endif // TRANSFER_H
