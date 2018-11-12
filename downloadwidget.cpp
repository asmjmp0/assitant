#include "downloadwidget.h"

#include<QApplication>
#include<QDebug>
#include<QModelIndex>
QString liswidgtcurrenttext;
downloadWidget::downloadWidget(QDialog *parent) : QDialog(parent)
{
    setWindowTitle("下载");
    inputLineEdit=new QLineEdit;
    findBtn=new QPushButton(tr("查询"));
    reloadBtn=new QPushButton(tr("←"));
    listWidget=new QListWidget;
    mainlayout=new QGridLayout(this);
    hlayout=new QHBoxLayout;
    dataini=new QSettings(QApplication::applicationDirPath()+"/data.ini", QSettings::IniFormat);
    hlayout->addWidget(findBtn);
    hlayout->addWidget(reloadBtn);
    mainlayout->addWidget(inputLineEdit,0,0);
    mainlayout->addLayout(hlayout,0,1);
    mainlayout->addWidget(listWidget,1,0,1,2);
    reloadpro();
    connect(findBtn,SIGNAL(clicked(bool)),this,SLOT(findpro()));
    connect(reloadBtn,SIGNAL(clicked(bool)),this,SLOT(reloadpro()));
    connect(listWidget,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(addtosidewidget()));
    connect(listWidget,SIGNAL(clicked(QModelIndex)),this,SLOT(listclick()));
}
void downloadWidget::closeEvent(QCloseEvent *event)
{
    qDebug()<<event;
}
void downloadWidget::listclick()
{
    liswidgtcurrenttext=dataini->value("text/"+listWidget->currentItem()->text()).toString();
    viewwidget=new view;
    viewwidget->move(cursor().pos().x()+5,cursor().pos().y()+5);
    viewwidget->show();
}
void downloadWidget::addtosidewidget()
{
    viewwidget->hide();
    emit addsignal(listWidget->currentItem()->text(),dataini->value("text/"+listWidget->currentItem()->text()).toString());
}
void downloadWidget::findpro()
{
    int i=0;
    while(i!=listWidget->count())
    {
        qDebug()<<listWidget->item(i)->text();
        if(!listWidget->item(i)->text().contains(inputLineEdit->text())) listWidget->takeItem(i);
        else i++;
    }
    inputLineEdit->clear();
}
void downloadWidget::reloadpro()
{
    listWidget->clear();
    dataini->beginGroup("text");
    strlist=dataini->childKeys();
    dataini->endGroup();
    for(int i=0;i<strlist.count();i++)
        listWidget->addItem(strlist.at(i));
}
downloadWidget::~downloadWidget()
{

}
