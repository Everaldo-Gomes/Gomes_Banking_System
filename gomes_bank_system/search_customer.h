#ifndef SEARCH_CUSTOMER_H
#define SEARCH_CUSTOMER_H

#include <QWidget>

namespace Ui {
class search_customer;
}

class search_customer : public QWidget
{
    Q_OBJECT

public:
    explicit search_customer(QWidget *parent = nullptr);
    ~search_customer();

private:
    Ui::search_customer *ui;
};

#endif // SEARCH_CUSTOMER_H
