#include "login.h"
#include "main_screen.h"
#include "search_staff.h"
#include "search_customer.h"
#include "search_account.h"
#include "about.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication system(argc, argv);
    Login login_obj;
    //login_obj.show();

    //main_screen *obj_main_screen = new main_screen();
    //obj_main_screen->show();

    /*are not implemented yet*/
    search_staff *sf = new search_staff();
    sf->show();

    //search_customer *sc = new search_customer();
    //sc->show();

    //search_account *sa = new search_account();
    //sa->show();

    return system.exec();
}
