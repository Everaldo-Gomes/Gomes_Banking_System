QT       += core gui
QT       += sql
QT += printsupport #it has to be here to here to print a file

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
    add_customer_account.cpp \
    add_staff.cpp \
    block_customer_account.cpp \
    block_staff.cpp \
    change_customer_info.cpp \
    change_staff_info.cpp \
    check_balance.cpp \
    database_connection.cpp \
    deposit.cpp \
    loan.cpp \
    main.cpp \
    login.cpp \
    main_screen.cpp \
    pay_loan.cpp \
    search_customer_account.cpp \
    search_loan.cpp \
    search_staff.cpp \
    support.cpp \
    transfer.cpp \
    unlock_customer_account.cpp \
    unlock_staff.cpp \
    withdrawal.cpp

HEADERS += \
    about.h \
    add_customer_account.h \
    add_staff.h \
    block_customer_account.h \
    block_staff.h \
    change_customer_info.h \
    change_staff_info.h \
    check_balance.h \
    database_connection.h \
    deposit.h \
    global_variables.h \
    loan.h \
    login.h \
    main_screen.h \
    pay_loan.h \
    search_customer_account.h \
    search_loan.h \
    search_staff.h \
    support.h \
    transfer.h \
    unlock_customer_account.h \
    unlock_staff.h \
    withdrawal.h

FORMS += \
    about.ui \
    add_customer_account.ui \
    add_staff.ui \
    block_customer_account.ui \
    block_staff.ui \
    change_customer_info.ui \
    change_staff_info.ui \
    check_balance.ui \
    deposit.ui \
    loan.ui \
    login.ui \
    main_screen.ui \
    pay_loan.ui \
    search_customer_account.ui \
    search_loan.ui \
    search_staff.ui \
    support.ui \
    transfer.ui \
    unlock_customer_account.ui \
    unlock_staff.ui \
    withdrawal.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

unix|win32: LIBS += -lpq

DISTFILES +=
