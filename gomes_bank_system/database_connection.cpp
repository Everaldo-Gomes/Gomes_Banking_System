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

int search_id_by_cpf(QString typed_cpf) {
    int staff_id;
    QSqlQuery check_id;
    check_id.exec("SELECT id FROM staff WHERE cpf = '"+typed_cpf+"';"); //passing a string variable in a query

    while(check_id.next()) { staff_id = check_id.value(0).toInt(); }
    return staff_id;
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
    int staff_id = search_id_by_cpf(typed_cpf);

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

bool blocked_many_times(int blocked_staff_id) {
    bool was_blocked = false;

    //check if the id is was blocked at leat once
    QSqlQuery search_query;
    search_query.exec("SELECT mt.staff_id from many_times_staff_blocked mt join staff st on st.id = mt.staff_id");

    while(search_query.next()) {
        int found_id = search_query.value(0).toInt();
        if(found_id == blocked_staff_id) {
            was_blocked = true;
            break;
        }
    }
    return was_blocked;
}

void block_staff_(QString connected_id, int found_id_int, QString reason_message) {
    QSqlQuery block_staff_query;
    block_staff_query.prepare("INSERT INTO blocked_staff (responsible_staff_id, blocked_staff_id, blocking_day, reason)"
                              "VALUES (?,?,?,?)");

    block_staff_query.addBindValue(connected_id.toInt());
    block_staff_query.addBindValue(found_id_int);
    block_staff_query.addBindValue(QDateTime::currentDateTime());
    block_staff_query.addBindValue(reason_message);
    block_staff_query.exec();
}

int how_many_times_blocked(int staff_id) {
    QSqlQuery get_value;
    get_value.prepare("SELECT times FROM many_times_staff_blocked WHERE staff_id = ?");
    get_value.addBindValue(staff_id);
    get_value.exec();

    int count;
    while(get_value.next()) { count = get_value.value(0).toInt(); }
    return count;
}

void update_qnt_times_blocked(int staff_id) {
    int count = how_many_times_blocked(staff_id);

    //add by 1
    QSqlQuery count_blocked_staff;
    count_blocked_staff.prepare("UPDATE many_times_staff_blocked set times = ? WHERE staff_id = ?");
    count_blocked_staff.addBindValue(count+1);
    count_blocked_staff.addBindValue(staff_id);
    count_blocked_staff.exec();
}

bool search_customer_cpf(QString typed_cpf) {
    bool not_found = true;
    QSqlQuery search_cpf;

    search_cpf.exec("SELECT cpf FROM customer;");

    while(search_cpf.next()) {
        QString found_cpf = search_cpf.value(0).toString();
        if(typed_cpf == found_cpf) { not_found = false; }
    }
    return not_found;
}

int search_customer_id_by_cpf(QString typed_cpf) {
    int customer_id;
    QSqlQuery check_id;
    check_id.exec("SELECT id FROM customer WHERE cpf = '"+typed_cpf+"';"); //passing a string variable in a query

    while(check_id.next()) { customer_id = check_id.value(0).toInt(); }
    return customer_id;
}




