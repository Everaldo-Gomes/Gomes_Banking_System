#include "database_connection.h"

QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL");

bool connect_database() {

    db.setHostName("localhost");
    db.setDatabaseName("gomes_bank_system");
    db.setUserName("main");
    db.setPassword("gomes");
    db.open();

    if(db.open()) { return true;  }
    else          { return false; }
}

void close_connection_database() {
    db.close();
}

bool search_cpf(QString typed_cpf) {

    bool not_found = true;
    QSqlQuery search_cpf;

    search_cpf.exec("SELECT cpf FROM staff;");

    while(search_cpf.next()) {
        QString found_cpf = search_cpf.value(0).toString();
        if(typed_cpf == found_cpf) { not_found = false; }
    }
    return not_found;
}

bool staff_blocked(QString typed_cpf) {
    bool is_blocked = false;
    int staff_id;

    //search id by using typed_cpf
    QSqlQuery check_id;
    check_id.exec("SELECT id FROM staff where cpf = '"+typed_cpf+"';"); //passing variable as a query

    while(check_id.next()) { staff_id = check_id.value(0).toInt(); }

    //check if the id is in the blocked table
    QSqlQuery id_blocked;
    id_blocked.exec("SELECT bs.blocked_staff_id FROM blocked_staff bs join staff st on st.id = bs.blocked_staff_id;");

    while(id_blocked.next()) {
        int found_id = id_blocked.value(0).toInt();
        if(found_id == staff_id) {
            is_blocked = true;
            break;
        }
    }
    return is_blocked;
}

void block_staff_(QString connected_id, int found_id_int, QString reason_message) {
    QSqlQuery block_staff_query;
    block_staff_query.prepare("INSERT INTO blocked_staff (responsable_staff_id, blocked_staff_id, blocking_day, reason)"
                              "VALUES (?,?,?,?)");

    block_staff_query.addBindValue(connected_id.toInt());
    block_staff_query.addBindValue(found_id_int);
    block_staff_query.addBindValue(QDateTime::currentDateTime());
    block_staff_query.addBindValue(reason_message);
    block_staff_query.exec();
}










