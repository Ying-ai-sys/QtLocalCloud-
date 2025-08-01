#ifndef SHAREFILE_H
#define SHAREFILE_H

#include <QWidget>

namespace Ui {
class Sharefile;
}

class Sharefile : public QWidget
{
    Q_OBJECT

public:
    explicit Sharefile(QWidget *parent = nullptr);
    ~Sharefile();
    void updataFriend_LW();

private slots:

    void on_allSelected_PB_clicked();

    void on_cancelSelected_PB_clicked();

    void on_ok_PB_clicked();

private:
    Ui::Sharefile *ui;
};

#endif // SHAREFILE_H
