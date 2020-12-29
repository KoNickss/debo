#ifndef DEBO_H
#define DEBO_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class debo; }
QT_END_NAMESPACE

class debo : public QMainWindow
{
    Q_OBJECT

public:
    debo(QWidget *parent = nullptr);
    ~debo();
    bool done=false;

private slots:
    void on_pushButton_clicked();
    void delay();

private:
    Ui::debo *ui;

};
#endif // DEBO_H
