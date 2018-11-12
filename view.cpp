#include "view.h"
#include"windows.h"
#include<QDebug>
#define number 7
extern QString liswidgtcurrenttext;
view::view(QDialog*parent) : QDialog(parent)
{
    edit=new QTextEdit;
    mainlayout=new QVBoxLayout(this);
    mainlayout->addWidget(edit);
    mainlayout->setMargin(0);
    setWindowFlags(Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint);
    edit->setEnabled(false);
    edit->setText(liswidgtcurrenttext);
    timer1 = new QTimer(this);
    timer1->start(100);
    connect(timer1,SIGNAL(timeout()),this,SLOT(onTimeout()));

}
void view::onTimeout()
{
    POINT p;
    GetCursorPos(&p);
    if(p.x<geometry().x()-number||p.x>geometry().x()+geometry().width()+number||p.y<geometry().y()-number||p.y>geometry().y()+geometry().height()+number)
        {
        timer1->stop();
        this->close();
        }
}
void view::closeEvent(QCloseEvent *event)
{
    //qDebug()<<event;
}
view::~view()
{

}
