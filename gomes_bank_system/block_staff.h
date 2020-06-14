#ifndef BLOCK_STAFF_H
#define BLOCK_STAFF_H

#include <QWidget>

namespace Ui {
class block_staff;
}

class block_staff : public QWidget
{
    Q_OBJECT

public:
    explicit block_staff(QWidget *parent = nullptr);
    ~block_staff();

private slots:
    void on_search_button_clicked();

private:
    Ui::block_staff *ui;
};

#endif // BLOCK_STAFF_H
