#ifndef DATABASE_CONNECTION_H
#define DATABASE_CONNECTION_H

#include <QSqlError>
#include <QIntValidator>
#include <QCoreApplication>
#include <QSqlDatabase>
#include <QtSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QIntValidator>

bool connect_database();               //open connection if the database

void close_connection_database();      //close the connection to the database

//==about staff

bool search_cpf(QString typed_cpf);    //verify if the staff alrady exist

bool staff_blocked(QString typed_cpf);  //verify if the staff is blocked

void block_staff_(QString connected_id, int found_id_int, QString reason_message);  //block staff and put information into the data base


//==about customer/account

#endif // DATABASE_CONNECTION_H
