#ifndef SEARCH_ACCOUNT_H
#define SEARCH_ACCOUNT_H

#include <QWidget>

namespace Ui {
class search_account;
}

class search_account : public QWidget
{
    Q_OBJECT

public:
    explicit search_account(QWidget *parent = nullptr);
    ~search_account();

private:
    Ui::search_account *ui;
};

#endif // SEARCH_ACCOUNT_H
