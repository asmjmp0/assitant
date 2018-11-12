#include "assistant.h"
#include<QDebug>
#include<QMessageBox>
#include<QApplication>
#include<QKeyEvent>
int count=0;
bool status=false;
bool findit= false;
bool self;
bool sideshowed=false;
bool sidefixed=false;
bool sideenable;
bool anime=true;
bool opflag=false;
int savetime;
int sideOpacity;
QString name;
QString sidefontcolor;
QSettings *m_IniFile;
extern QString textstr;
assistant::assistant(QWidget *parent)
    : QWidget(parent)
{
    screenBtn=new QPushButton(tr("截屏"));
    screenBtn->setStyleSheet("background-color: blue");
    lockBtn=new QPushButton(tr("多任务"));
    lockBtn->setStyleSheet("background-color: red");
    cmdlineedit=new QLineEdit;
    browserBtn=new QPushButton(tr("浏览器"));
    browserBtn->setStyleSheet("background-color: green");
    optionBtn=new QPushButton(tr("介绍与选项"));
    optionBtn->setStyleSheet("background-color: yellow");
    mainlayout=new QGridLayout(this);
    mainlayout->addWidget(screenBtn,0,0);
    mainlayout->addWidget(lockBtn,0,1);
    mainlayout->addWidget(cmdlineedit,1,0,1,2);
    mainlayout->addWidget(browserBtn,2,0);
    mainlayout->addWidget(optionBtn,2,1);
    mainlayout->setSpacing(50);

    m_IniFile = new QSettings(QApplication::applicationDirPath()+"/option.ini", QSettings::IniFormat);
    m_IniFile->setIniCodec(QTextCodec::codecForName("GB2312"));
    if(m_IniFile->value( "first/first").toString()=="" && m_IniFile->value( "first/first").toString()!="1")//第一次启动
    {
        m_IniFile->setValue( "first/first",1);
        m_IniFile->setValue( "self/self",1);
        m_IniFile->setValue("sidewidget/enable",1);
        m_IniFile->setValue("sidewidget/savatime",3);
        m_IniFile->setValue("sidewidget/Opacity",87);
        m_IniFile->setValue("sidewidget/fontcolor","yellow");
        m_IniFile->setValue("sidewidget/anime",1);
    }

   self=m_IniFile->value("self/self").toBool();
   savetime=m_IniFile->value("sidewidget/savatime").toInt();
   sideenable=m_IniFile->value("sidewidget/enable").toBool();
   sideOpacity=m_IniFile->value("sidewidget/Opacity").toInt();
   sidefontcolor=m_IniFile->value("sidewidget/fontcolor").toString();
   anime=m_IniFile->value("sidewidget/anime").toBool();

    this->resize(100,100);
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground, true);
    timer=new QTimer(this);
    timer->start(200);
    sysicon=new QSystemTrayIcon(this);
    QIcon icon= QIcon(":/new/prefix1/159.png");
    sysicon->setIcon(icon);
    sysicon->setToolTip(tr("小助手"));
    sysicon->show();

    side=new sidewidget;
    side->updatepro();
    side->setWindowOpacity((double)sideOpacity/100);
    op=new option;
    side->show();
    side->hide();

    cmdlineedit->installEventFilter(this);
    cmdlineedit->setFocus();

    connect(timer,SIGNAL(timeout()),this,SLOT(keystate()));
    connect(screenBtn,SIGNAL(clicked(bool)),this,SLOT(screen()));
    connect(lockBtn,SIGNAL(clicked(bool)),this,SLOT(locked()));
    connect(browserBtn,SIGNAL(clicked(bool)),this,SLOT(browser()));
    connect(optionBtn,SIGNAL(clicked(bool)),this,SLOT(optionpro()));
    connect(op,SIGNAL(setOpacity(int)),this,SLOT(tosetOpacity(int)));
    connect(op,SIGNAL(itleaveslider()),this,SLOT(toleaveslider()));
    connect(op,SIGNAL(fontcolor(QString)),this,SLOT(tosetfontcolor(QString)));
    connect(cmdlineedit,SIGNAL(returnPressed()),this,SLOT(cmdpro()));
    connect(cmdlineedit,SIGNAL(cursorPositionChanged(int,int)),this,SLOT(inputpro()));
    connect(sysicon,SIGNAL(activated(QSystemTrayIcon::ActivationReason)),this,SLOT(showpro(QSystemTrayIcon::ActivationReason)));



    createAciton();
    createMenu();
    selfrun();
    setfontcolor(sidefontcolor);
}
void assistant::showpro(QSystemTrayIcon::ActivationReason reason )
{
    if(reason==QSystemTrayIcon::Trigger)
        if(sidefixed)
        {
           side->show();
           SetWindowPos(HWND(side->winId()), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);//运用管理员权限
           SetWindowPos(HWND(side->winId()), HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
        }

}
void assistant::inputpro()
{
    status=false;
}
void assistant::cmdpro()
{
    hide();
    WinExec(cmdlineedit->text().toStdString().c_str(),SW_NORMAL);
    cmdlineedit->clear();
}
bool assistant::eventFilter(QObject *obj, QEvent *event)
{
    if(obj==cmdlineedit)
        if(event->type()==QEvent::MouseButtonPress)
            status=false;
        if(event->type()==QEvent::KeyPress)
        {
            QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
            if(keyEvent->key()==Qt::Key_Down)
                    browser();
        }

    return false;
}
void assistant::screen()
{
this->setFocus();
hide();
count=0;
status=false;
keybd_event(18,0,0,0);
keybd_event(VK_CONTROL,0,0,0);//ALT键
keybd_event(65,0,0,0);//A键
keybd_event(18,0,2,0);
keybd_event(VK_CONTROL,0,2,0);
keybd_event(65,0,2,0);
}
void assistant::locked()
{
hide();
count=0;
status=false;
keybd_event(VK_LWIN,0,0,0);
keybd_event(VK_TAB,0,0,0);
keybd_event(VK_LWIN,0,2,0);
keybd_event(VK_TAB,0,2,0);
}
void assistant::browser()
{
    hide();
    count=0;
    status=false;
    if(cmdlineedit->text()=="")
    QDesktopServices::openUrl(QUrl("www.baidu.com"));
    else
    {
        QString temp="www.baidu.com";
        temp=temp+"/s?wd="+cmdlineedit->text();
        QDesktopServices::openUrl(QUrl(temp));
        cmdlineedit->clear();
    }
}
void assistant::optionpro()
{
    this->hide();
    op->show();
}
void assistant::tosetOpacity(int value)
{
    if(!sideshowed)
    {
        side->move(0,0);
        side->show();
        sideshowed=true;
    }
    double temp=(double)value/100;
    side->setWindowOpacity(temp);
    sideOpacity=value;
    side->timer->stop();
    if(!sidefixed)
       side->timer->start(1000);
}
void assistant::toleaveslider()
{
    if(!sidefixed)
       side->timer->start(1000);
}
void assistant::tosetfontcolor(QString str)
{
    side->move(0,0);
    side->show();
    sideshowed=true;
    setfontcolor(str);
    if(!sidefixed)
        side->timer->start(1500);
}
void assistant::keystate()//时钟回调函数
{
    int vkey;
    double i;
    static int c=0;
    vkey=GetAsyncKeyState(VK_MBUTTON);
    if(cursor().pos().x()==0 && sideshowed==false && sideenable)//显示侧边栏
    {
        c++;
        if(c==3)
        {
            SetWindowPos(HWND(side->winId()), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);//运用管理员权限
            SetWindowPos(HWND(side->winId()), HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
            side->show();
            if(anime)
            {
            for(i=-500;i<0;i+=0.0002)//载入动画
            {
                side->move((int)i,0);
            }
            i=-500;
            }
            side->move(0,0);
            sideshowed=true;
            if(!sidefixed)
            side->timer->start(100);
        }
    }
    if(cursor().pos().x()!=0)
        c=0;
    if(vkey)//显示中键菜单
    {
        count=0;
       this->move(cursor().pos().x()-105,cursor().pos().y()-55);
        SetWindowPos(HWND(winId()), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);//运用管理员权限
        SetWindowPos(HWND(winId()), HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
       this->show();
       status=true;
    }
    if(status)
    {
        count++;
        if(count==14)
        {
            count=0;
            status=false;
            hide();
        }
    }
}
void assistant::optionAction()
{
    op->show();
}
void assistant::createAciton()
{
    ExitAction = new QAction(tr("退出并保存"),this);
        connect(ExitAction,SIGNAL(triggered()),this,SLOT(exitAction()));
    OptionAction =new QAction(tr("选项和介绍"),this);
        connect(OptionAction,SIGNAL(triggered()),this,SLOT(optionAction()));
}
void assistant::createMenu()
{
    mMenu=new QMenu(this);
    mMenu->addAction(ExitAction);
    mMenu->addSeparator();
    mMenu->addAction(OptionAction);
    sysicon->setContextMenu(mMenu);
}
void assistant::exitAction()
{
    m_IniFile->setValue("text/"+side->clcbox->currentText(),side->tEdit->toPlainText());
    sysicon->hide();
    close();
    side->close();
    op->close();
}
void assistant::selfrun()
{


           QString pathstr="HKEY_LOCAL_MACHINE\\Software\\Microsoft\\Windows\\CurrentVersion\\Run";
           QStringList list;
           QSettings *preg=new QSettings(pathstr,QSettings::NativeFormat);
           list=preg->childKeys();
           for(int i=0;i<list.count();i++)
           {
               if(list.at(i)=="assistant")
                   findit=true;

           }
           if(!findit && self)
           {
               preg->setValue("assistant","\""+name+"\"");
           }
}
void assistant::setfontcolor(QString str)
{
    if(str=="white")
    {
        side->fmt.setForeground(Qt::white);
        side->updatepro();
        return;
    }
    if(str=="red")
    {
        side->fmt.setForeground(Qt::red);
        side->updatepro();
        return;
    }
    if(str=="magenta")
    {
        side->fmt.setForeground(Qt::magenta);
        side->updatepro();
        return;
    }
    if(str=="black")
    {
        side->fmt.setForeground(Qt::black);
        side->updatepro();
        return;
    }
    if(str=="yellow")
    {
        side->fmt.setForeground(Qt::yellow);
        side->updatepro();
        return;
    }
    if(str=="blue")
    {
        side->fmt.setForeground(Qt::blue);
        side->updatepro();
        return;
    }
    if(str=="gray")
    {
        side->fmt.setForeground(Qt::gray);
        side->updatepro();
        return;
    }
    if(str=="green")
    {
        side->fmt.setForeground(Qt::green);
        side->updatepro();
        return;
    }
    if(str=="lightGray")
    {
        side->fmt.setForeground(Qt::lightGray);
        side->updatepro();
        return;
    }
}
assistant::~assistant()
{
timer->stop();
}
