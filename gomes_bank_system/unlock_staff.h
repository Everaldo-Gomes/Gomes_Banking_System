#ifndef UNLOCK_STAFF_H
#define UNLOCK_STAFF_H

#include <QWidget>
#include <QMessageBox>
#include <QDebug>
#include "database_connection.h"
#include "global_variables.h"

namespace Ui {
class unlock_staff;
}

class unlock_staff : public QWidget
{
    Q_OBJECT

public:
    explicit unlock_staff(QWidget *parent = nullptr);
    ~unlock_staff();

private slots:
    void on_cpf_field_input_textChanged();

    void on_search_button_clicked();

    void on_reason_message_input_textChanged();

    void on_unlock_button_clicked();

    void on_cancel_button_clicked();

private:
    Ui::unlock_staff *ui;
};

#endif // UNLOCK_STAFF_H
