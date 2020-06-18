QT       += core gui
QT       += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    about.cpp \
    add_staff.cpp \
    block_staff.cpp \
    change_staff_info.cpp \
    database_connection.cpp \
    main.cpp \
    login.cpp \
    main_screen.cpp \
    search_account.cpp \
    search_customer.cpp \
    search_staff.cpp \
    support.cpp \
    unlock_staff.cpp

HEADERS += \
    about.h \
    add_staff.h \
    block_staff.h \
    change_staff_info.h \
    database_connection.h \
    global_variables.h \
    login.h \
    main_screen.h \
    search_account.h \
    search_customer.h \
    search_staff.h \
    support.h \
    unlock_staff.h

FORMS += \
    about.ui \
    add_staff.ui \
    block_staff.ui \
    change_staff_info.ui \
    login.ui \
    main_screen.ui \
    search_account.ui \
    search_customer.ui \
    search_staff.ui \
    support.ui \
    unlock_staff.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

unix|win32: LIBS += -lpq

DISTFILES +=
