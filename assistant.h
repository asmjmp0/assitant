#ifndef ASSISTANT_H
#define ASSISTANT_H

#include <QWidget>
#include<QPushButton>
#include<QGridLayout>
#include<QTimer>
#include<QLineEdit>
#include<windows.h>
#include<QSystemTrayIcon>
#include<QMenu>
#include<QAction>
#include"option.h"
#include"sidewidget.h"
#include<QSettings>
#include <QDesktopServices>
#include <QUrl>
#include<QTextCodec>
#include<QObject>
#include<QEvent>
class assistant : public QWidget
{
    Q_OBJECT

public:
    assistant(QWidget *parent = 0);
    ~assistant();
    void createMenu();
    void createAciton();
    void selfrun();
    void setfontcolor(QString);
private:
    QTimer *timer;
    QPushButton *lockBtn;
    QPushButton *screenBtn;
    QLineEdit *cmdlineedit;
    QPushButton *browserBtn;
    QPushButton *optionBtn;
    QGridLayout *mainlayout;
    QSystemTrayIcon *sysicon;
    QMenu *mMenu;
    QAction *ExitAction;
    QAction *OptionAction;
    option *op;
    sidewidget *side;
private slots:
    void locked();
    void screen();
    void keystate();
    void exitAction();
    void optionAction();
    void browser();
    void optionpro();
    void tosetOpacity(int);
    void toleaveslider();
    void tosetfontcolor(QString);
    void cmdpro();
    void inputpro();
    void showpro(QSystemTrayIcon::ActivationReason);
protected:
    bool eventFilter(QObject *obj, QEvent *event);
};

#endif // ASSISTANT_H
