#include "login.h"
#include "main_screen.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication system(argc, argv);
    Login login_obj;
    login_obj.show();

    //main_screen *obj_main_screen = new main_screen();
    //obj_main_screen->show();

    return system.exec();
}
