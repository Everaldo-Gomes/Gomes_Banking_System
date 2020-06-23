#include "login.h"
#include "main_screen.h"
#include "search_staff.h"
#include "search_customer_account.h"
#include "about.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication system(argc, argv);
    Login login_obj;
    //login_obj.show();

    //main_screen *obj_main_screen = new main_screen();
    //obj_main_screen->show();

    search_customer_account *sca = new search_customer_account;
    sca->show();

    return system.exec();
}
