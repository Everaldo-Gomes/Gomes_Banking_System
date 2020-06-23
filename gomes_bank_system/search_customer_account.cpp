#include "search_customer_account.h"
#include "ui_search_customer_account.h"

search_customer_account::search_customer_account(QWidget *parent) : QWidget(parent), ui(new Ui::search_customer_account) {
    ui->setupUi(this);
}

search_customer_account::~search_customer_account()
{
    delete ui;
}
