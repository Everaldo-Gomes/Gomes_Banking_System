#ifndef SEARCH_STAFF_H
#define SEARCH_STAFF_H

#include <QWidget>
#include <QIntValidator>

namespace Ui {
class search_staff;
}

class search_staff : public QWidget
{
    Q_OBJECT

public:
    explicit search_staff(QWidget *parent = nullptr);
    ~search_staff();

private slots:

    void on_close_button_clicked();

    void on_search_again_button_clicked();

private:
    Ui::search_staff *ui;
};

#endif // SEARCH_STAFF_H
