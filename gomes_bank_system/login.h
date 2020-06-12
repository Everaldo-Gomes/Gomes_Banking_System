#ifndef LOGIN_H
#define LOGIN_H

#include <QMainWindow>
#include "main_screen.h"
#include <global_variables.h>

QT_BEGIN_NAMESPACE
namespace Ui { class Login; }
QT_END_NAMESPACE

class Login : public QMainWindow
{
    Q_OBJECT

public:
    Login(QWidget *parent = nullptr);
    ~Login();

private slots:

    void on_cpf_field_textEdited();

    void on_password_field_textEdited();

    void on_login_button_clicked();

    void on_cancel_button_clicked();

private:
    Ui::Login *ui;
};
#endif // LOGIN_H
