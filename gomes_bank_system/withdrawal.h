#ifndef WITHDRAWAL_H
#define WITHDRAWAL_H

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
class withdrawal;
}

class withdrawal : public QWidget
{
    Q_OBJECT

public:
    explicit withdrawal(QWidget *parent = nullptr);
    ~withdrawal();

private slots:
    void on_close_button_clicked();

    void on_withdrawal_button_clicked();

    void on_cpf_input_textChanged();

    void on_account_input_textChanged();

    void on_value_input_textChanged();

    void on_password_input_textChanged();

    void on_name_input_textChanged();

private:
    Ui::withdrawal *ui;
};

#endif // WITHDRAWAL_H
