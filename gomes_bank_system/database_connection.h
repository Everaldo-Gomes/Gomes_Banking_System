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
bool search_cpf(QString typed_cpf); //verify if the staff already exist

int search_id_by_cpf(QString typed_cpf); //search id using the cpf

bool staff_blocked(QString typed_cpf);  //verify if the staff is blocked

bool blocked_many_times(int blocked_staff_id); //verify if the staff already is in the may_times_blocked_staff database

void block_staff_(QString connected_id, int found_id_int, QString reason_message); //block staff and put information into the blocked_staff database

int how_many_times_blocked(int staff_id); //return how many times a staff was blocked

void update_qnt_times_blocked(int staff_id); //add by 1 the number of times the staff was blocked


//==about customer & account
bool search_customer_cpf(QString typed_cpf); //verify if the customer already exist

int search_customer_id_by_cpf(QString typed_cpf); //search id using the cpf

QString get_acc_number(QString typed_cpf); //return the account number

int get_acc_id(int cust_id); //return the acount id

void block_customer_acc(QString connected_id, int found_id_int, QString reason_message); //block customer/account and put information into the database

bool customer_blocked(QString typed_cpf); //verify if the customer is blocked using cpf

bool customer_blocked_acc(QString typed_account); //verify if the customer is blocked using account number

bool blocked_customer_acc_many_times(int blocked_customer_id); //verify if the customer already is in the may_times_customer_account blocked_ database

int how_many_times_customer_acc_blocked(int customer_id); //return how many times the customer/account was blocked

void update_qnt_customer_acc_blocked(int customer_id); //add by 1 the number of times the staff & account ware blocked

QString get_account_amount(int cus_id); //return the amount

void add_amount(double amount, QString typed_cpf); //add some value in the amount

QString loan_paid_any(QString loan_key);


//not related to the database (need to find another place to declare it)
bool has_decimal_point(QString typed_amount); //check if the number has decimal point

#endif // DATABASE_CONNECTION_H
