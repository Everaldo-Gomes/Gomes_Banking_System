#ifndef SEARCH_CUSTOMER_ACCOUNT_H
#define SEARCH_CUSTOMER_ACCOUNT_H

#include <QWidget>

namespace Ui {
class search_customer_account;
}

class search_customer_account : public QWidget
{
    Q_OBJECT

public:
    explicit search_customer_account(QWidget *parent = nullptr);
    ~search_customer_account();

private:
    Ui::search_customer_account *ui;
};

#endif // SEARCH_CUSTOMER_ACCOUNT_H
