#ifndef DOWNLOADWIDGET_H
#define DOWNLOADWIDGET_H

#include <QDialog>
#include<QLineEdit>
#include<QPushButton>
#include<QListWidget>
#include<QSettings>
#include<QGridLayout>
#include<QCloseEvent>
#include"view.h"
class downloadWidget : public QDialog
{
    Q_OBJECT
private:
    QLineEdit *inputLineEdit;
    QPushButton *findBtn;
    QPushButton *reloadBtn;
    QListWidget *listWidget;
    QSettings *dataini;
    QGridLayout *mainlayout;
    QHBoxLayout *hlayout;
    QStringList strlist;
    view *viewwidget;
public:
    explicit downloadWidget(QDialog *parent = nullptr);
    ~downloadWidget();
private slots:
    void findpro();
    void reloadpro();
    void addtosidewidget();
    void listclick();
signals:
    void addsignal(QString,QString);
protected:
    void closeEvent(QCloseEvent *event);
};

#endif // DOWNLOADWIDGET_H
