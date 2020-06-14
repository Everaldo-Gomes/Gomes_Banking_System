#ifndef SUPPORT_H
#define SUPPORT_H

#include <QWidget>
#include <QDebug>

#include <QSqlDatabase>
#include <QtSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QIntValidator>
#include <QDateTime>
#include <QMessageBox>
#include <QDebug>
#include "global_variables.h"


namespace Ui {
class support;
}

class support : public QWidget
{
    Q_OBJECT

public:
    explicit support(QWidget *parent = nullptr);
    ~support();

private slots:
    void on_support_message_textChanged();

    void on_cancel_button_clicked();

    void on_send_button_clicked();

private:
    Ui::support *ui;
};

#endif // SUPPORT_H
