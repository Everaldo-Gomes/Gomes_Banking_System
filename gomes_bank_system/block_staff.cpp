#include "block_staff.h"
#include "ui_block_staff.h"

block_staff::block_staff(QWidget *parent) : QWidget(parent), ui(new Ui::block_staff) {
    ui->setupUi(this);
    this->setFixedHeight(100); //start the window in this size


}

block_staff::~block_staff() {
    delete ui;
}

void block_staff::on_search_button_clicked() {
    this->setFixedHeight(400);
    this->move(480, 200);
}
