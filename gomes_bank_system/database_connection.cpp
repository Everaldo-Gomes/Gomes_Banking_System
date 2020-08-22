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
    search_query.exec("SELECT mt.staff_id FROM many_times_staff_blocked mt JOIN staff st ON st.id = mt.staff_id");

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

QString get_acc_number(QString typed_cpf) {
    QString acc_n;
    QSqlQuery get_acc_num;
    get_acc_num.exec("SELECT an.account_number FROM account an JOIN customer ct ON ct.id = an.customer_id WHERE ct.cpf = '"+typed_cpf+"';");
    while(get_acc_num.next()) { acc_n = get_acc_num.value(0).toString(); }
    return acc_n;
}

int get_acc_id(int cust_id) {
    int acc_id;
    QSqlQuery get_acc_id;
    get_acc_id.prepare("select acc.id from account acc join customer ct on ct.id = acc.customer_id where acc.customer_id = ?");
    get_acc_id.addBindValue(cust_id);
    get_acc_id.exec();
    while(get_acc_id.next()) { acc_id = get_acc_id.value(0).toInt(); }
    return acc_id;
}
void block_customer_acc(QString connected_id, int found_id_int, QString reason_message) {
    QSqlQuery block_staff_query;
    block_staff_query.prepare("INSERT INTO blocked_customer_account (responsible_staff_id, blocked_customer_id, blocking_day, reason)"
                              "VALUES (?,?,?,?)");
    block_staff_query.addBindValue(connected_id.toInt());
    block_staff_query.addBindValue(found_id_int);
    block_staff_query.addBindValue(QDateTime::currentDateTime());
    block_staff_query.addBindValue(reason_message);
    block_staff_query.exec();
}

bool blocked_customer_acc_many_times(int blocked_customer_id) {
    bool was_blocked = false;

    //check if the id is was blocked at leat once
    QSqlQuery search_query;
    search_query.exec("SELECT mtcab.customer_id FROM many_times_customer_account_blocked mtcab JOIN customer ct ON ct.id = mtcab.customer_id");

    while(search_query.next()) {
        int found_id = search_query.value(0).toInt();
        if(found_id == blocked_customer_id) {
            was_blocked = true;
            break;
        }
    }
    return was_blocked;
}

bool customer_blocked(QString typed_cpf) {
    bool is_blocked = false;
    int customer_id = search_customer_id_by_cpf(typed_cpf);

    //check if the id is in the blocked table
    QSqlQuery id_blocked;
    id_blocked.exec("SELECT bca.blocked_customer_id FROM blocked_customer_account bca join customer ct on ct.id = bca.blocked_customer_id;");

    while(id_blocked.next()) {
        int found_id = id_blocked.value(0).toInt();
        if(found_id == customer_id) {
            is_blocked = true;
            break;
        }
    }
    return is_blocked;
}

bool customer_blocked_acc(QString typed_account) {

    //get the cpf using account number
    QSqlQuery get_cpf;
    get_cpf.prepare("SELECT ct.cpf FROM customer ct JOIN account acc ON acc.customer_id = ct.id WHERE acc.account_number = ?");
    get_cpf.addBindValue(typed_account.toInt());
    get_cpf.exec();

    QString found_cpf;
    while(get_cpf.next()) { found_cpf = get_cpf.value(0).toString(); }

    //function that checks if the customer is blocked by using the cpf
    return customer_blocked(found_cpf);
}

int how_many_times_customer_acc_blocked(int customer_id) {
    QSqlQuery get_value;
    get_value.prepare("SELECT times FROM many_times_customer_account_blocked WHERE customer_id = ?");
    get_value.addBindValue(customer_id);
    get_value.exec();

    int count = 0;
    while(get_value.next()) { count = get_value.value(0).toInt(); }
    return count;
}

void update_qnt_customer_acc_blocked(int customer_id) {
    int count = how_many_times_customer_acc_blocked(customer_id);

    //add by 1
    QSqlQuery count_blocked;
    count_blocked.prepare("UPDATE many_times_customer_account_blocked SET times = ? WHERE customer_id = ?");
    count_blocked.addBindValue(count+1);
    count_blocked.addBindValue(customer_id);
    count_blocked.exec();
}

QString get_account_amount(int cus_id) {
    QString amount;
    QSqlQuery get_amount;
    get_amount.prepare("SELECT amount FROM account WHERE customer_id = ?");
    get_amount.addBindValue(cus_id);
    get_amount.exec();

    while(get_amount.next()) { amount = get_amount.value(0).toString(); }
    return amount;
}

void add_amount(double amount, QString typed_cpf) {
    QSqlQuery add;
    add.prepare("UPDATE account SET amount = ? WHERE account_number = ?");
    add.addBindValue(amount);
    add.addBindValue(get_acc_number(typed_cpf));
    add.exec();
}

QString loan_paid_any(QString loan_key) {
    QString is_paid = "";
    QSqlQuery search_payemnt;
    search_payemnt.exec("SELECT payment_installments_paid FROM loan_payment_control WHERE loan_receipt_key = '"+loan_key+"';");
    while(search_payemnt.next()) {
        is_paid = search_payemnt.value(0).toString();
    }
    return is_paid;
}

bool has_decimal_point(QString typed_amount) {
    for(int i = 0; i < typed_amount.length(); i++) {
        if(typed_amount[i] != ".") { continue; }
        else { return true; }
    }
    return false;
}
