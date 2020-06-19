#ifndef SEARCH_STAFF_H
#define SEARCH_STAFF_H

#include <QWidget>
#include <QIntValidator>
#include <QMessageBox>
#include <QDebug>
#include "database_connection.h"
#include "global_variables.h"

namespace Ui {
class search_staff;
}

class search_staff : public QWidget
{
    Q_OBJECT

public:
    explicit search_staff(QWidget *parent = nullptr);
    ~search_staff();

private slots:

    void on_close_button_clicked();

    void on_search_button_clicked();

    void on_cpf_field_input_textChanged();

private:
    Ui::search_staff *ui;
};

#endif // SEARCH_STAFF_H
