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

bool connect_database(); //open connection to the database
void close_connection_database();  //close the connection to the database


//==about staff
bool search_cpf(QString typed_cpf); //verify if the staff alrady exist

int search_id_by_cpf(QString typed_cpf); //search id using the cpf

bool staff_blocked(QString typed_cpf);  //verify if the staff is blocked

bool blocked_many_times(int blocked_staff_id); //verify if the staff already is in the may_times_blocked_staff database

void block_staff_(QString connected_id, int found_id_int, QString reason_message); //block staff and put information into the blocked_staff database

int how_many_times_blocked(int staff_id); //return how many times a staff was blocked



//==about customer/account

#endif // DATABASE_CONNECTION_H
