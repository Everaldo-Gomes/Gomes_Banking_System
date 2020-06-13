#include "search_customer.h"
#include "ui_search_customer.h"

search_customer::search_customer(QWidget *parent) : QWidget(parent), ui(new Ui::search_customer) {
    ui->setupUi(this);
}

search_customer::~search_customer() {
    delete ui;
}
