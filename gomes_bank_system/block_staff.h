#ifndef BLOCK_STAFF_H
#define BLOCK_STAFF_H

#include <QWidget>
#include <QMessageBox>
#include <QDebug>
#include "database_connection.h"
#include "global_variables.h"

namespace Ui {
class block_staff;
}

class block_staff : public QWidget
{
    Q_OBJECT

public:
    explicit block_staff(QWidget *parent = nullptr);
    ~block_staff();

private slots:
    void on_search_button_clicked();

    void on_cancel_button_clicked();

    void on_block_button_clicked();

    void on_cpf_field_input_textChanged();

    void on_reason_message_input_textChanged();

private:
    Ui::block_staff *ui;
};

#endif // BLOCK_STAFF_H
