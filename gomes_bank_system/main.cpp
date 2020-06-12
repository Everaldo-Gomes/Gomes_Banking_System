#include "login.h"
#include "main_screen.h"
#include "search_staff.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    Login login_obj;
    //login_obj.show();


    //main_screen *obj_main_screen = new main_screen();
    //obj_main_screen->show();

    search_staff *sf = new search_staff();
    sf->show();

    return a.exec();
}
