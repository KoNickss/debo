#include "debo.h"
#include "./ui_debo.h"
#include <QProcess>
#include <QFile>
#include <QTextStream>
#include <QtDebug>
#include <QDebug>
#include <QTime>

debo::debo(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::debo)
{
    ui->setupUi(this);
    system("mkdir /tmp/debo");
    system("cd /tmp/debo ; /usr/share/debo/ex " + QCoreApplication::arguments().at(1).toUtf8());
    system("mkdir /tmp/debo/control ; cd /tmp/debo/control ; /usr/share/debo/ex ../control.tar.*");
    //system("mkdir /tmp/debo/data ; cd /tmp/debo/data ; /usr/share/debo/ex ../data.tar.*");
    system("cd /tmp/debo/control ; cat /tmp/debo/control/control | grep Package > namae");
    QFile namae("/tmp/debo/control/namae");
    if(!namae.open(QIODevice::ReadOnly)){
    }
    QTextStream namep(&namae);
    QString packname;
    namep >> packname;
    namep >> packname;
    namae.close();
    ui->label->setText(packname);
    system("cd /tmp/debo/control ; cat /tmp/debo/control/control | grep Description > namae");
    if(!namae.open(QIODevice::ReadOnly)){
    }
    QTextStream namepd(&namae);
    QString packnamed;
    packnamed=namepd.readAll();
    namae.close();
    ui->label_2->setText(packnamed);
    system("cd /tmp/debo/control ; cat /tmp/debo/control/control | grep Maintainer > namae");
    if(!namae.open(QIODevice::ReadOnly)){
    }
    packnamed = namep.readAll();
    ui->label_2->setText(ui->label_2->text()+packnamed);
    namae.close();
    system("cd /tmp/debo/control ; cat /tmp/debo/control/control | grep Installed-Size > namae");
    if(!namae.open(QIODevice::ReadOnly)){
    }
    namep >> packnamed;
    QString test;
    namep >> test;
    double a = (double)test.toDouble()/1000;
    packnamed +=" " + QString::number(a) + " mb";
    ui->label_2->setText(ui->label_2->text()+packnamed);
    namae.close();
    system("cd /tmp/debo/control ; cat /tmp/debo/control/control | grep Section > namae");
    if(!namae.open(QIODevice::ReadOnly)){
    }
    packnamed = namep.readAll();
    ui->label_2->setText(ui->label_2->text()+packnamed);
    namae.close();
    system("cd /tmp/debo/control ; cat /tmp/debo/control/control | grep Version > namae");
    if(!namae.open(QIODevice::ReadOnly)){
    }
    packnamed = namep.readAll();
    ui->label_2->setText(ui->label_2->text()+packnamed);
    namae.close();
    system("cd /tmp/debo/control ; cat /tmp/debo/control/control | grep Depends > namae");
    if(!namae.open(QIODevice::ReadOnly)){
    }
    packnamed = namep.readAll();
    ui->label_3->setText(packnamed);
    namae.close();
}

debo::~debo()
{
    system("rm -rf /tmp/debo");
    delete ui;
}

void debo::delay()
{
    QTime dieTime= QTime::currentTime().addSecs(1);
    while (QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}
void debo::on_pushButton_clicked()
{
    if(done){
        exit(EXIT_FAILURE);
    }
    ui->tabWidget->setCurrentIndex(2);
    delay();
    ui->textBrowser->setText("");
    qDebug()<<"tc";
    //this reads the dependencies
    ui->textBrowser->setText(ui->textBrowser->toPlainText()+"Fetching dependency list. ");
    delay();
    QString adf = "/tmp/debo/control/namae";
    QFile fisier(adf);
    fisier.open(QIODevice::ReadOnly);
    QTextStream cit(&fisier);
    QString dep, fdep="";
    dep = cit.readAll();
    bool para=true;
    bool got_to_dep_part=false;
    int count=0;
    //this filters the dependencies into a string of args apt can use, there is probably a better way to do this but it works
    if(dep.at(0)=='D') count++;
    for(int i=0; i<dep.length()-1; i++){
        if(dep.at(i)=="(") para=false;
        if(dep.at(i)!=","&&para&&dep.at(i)!="|"&&got_to_dep_part)
            fdep += dep.at(i);
        if(dep.at(i)==")") para=true;
        if(dep.at(i)==":"){
            count++;
            if(count==2){
                got_to_dep_part=true;
            }
        }
    }
    qDebug() << fdep;
    ui->textBrowser->setText(ui->textBrowser->toPlainText()+"Requesting root permission. ");
    delay();
    QProcess askPermission;
    askPermission.start("kdialog --password \"Please enter your password\"");
    askPermission.waitForReadyRead(-1);
    QByteArray sudoPwd = askPermission.readAll().simplified();
    askPermission.close();
    ui->textBrowser->setText(ui->textBrowser->toPlainText()+"Getting dependencies from apt. ");
    delay();
    system("echo "+sudoPwd+" | sudo -S apt install "+fdep.toUtf8()+" > /tmp/debo/aptlog");
    ui->textBrowser->setText(ui->textBrowser->toPlainText()+"Writing data to disk. ");
    delay();
    system("echo "+sudoPwd+" | sudo -S dpkg -i "+QCoreApplication::arguments().at(1).toUtf8());
    ui->textBrowser->setText("Done!");
    ui->pushButton->setText("Close");
    done=true;
}
