#include "search_account.h"
#include "ui_search_account.h"

search_account::search_account(QWidget *parent) : QWidget(parent), ui(new Ui::search_account) {
    ui->setupUi(this);
}

search_account::~search_account() {
    delete ui;
}
