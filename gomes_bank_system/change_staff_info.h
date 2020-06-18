#ifndef CHANGE_STAFF_INFO_H
#define CHANGE_STAFF_INFO_H

#include <QWidget>
#include <QMessageBox>
#include <QDebug>
#include "database_connection.h"
#include "global_variables.h"
#include "login.h"


namespace Ui {
class change_staff_info;
}

class change_staff_info : public QWidget
{
    Q_OBJECT

public:
    explicit change_staff_info(QWidget *parent = nullptr);
    ~change_staff_info();

private slots:
    void on_cancel_button_clicked();

    void on_reason_message_input_textChanged();

    void on_search_button_clicked();

    void on_cpf_field_input_textChanged();

    void on_change_button_clicked();

private:
    Ui::change_staff_info *ui;
};

#endif // CHANGE_STAFF_INFO_H
