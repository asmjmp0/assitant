#include "sidewidget.h"
#include<qdebug.h>
#include<QCoreApplication>
#include<QApplication>
#include<QDesktopServices>
#include<QMessageBox>
extern bool sideshowed;
extern QString name;
extern bool sidefixed;
extern bool anime;
extern int savetime;
QString textstr;
extern QSettings *m_IniFile;
extern int sideOpacity;
extern bool opflag;
sidewidget::sidewidget(QWidget *parent) : QWidget(parent)
{
    this->setStyleSheet("background-color:green");
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setFixedSize(500,GetSystemMetrics(SM_CYFULLSCREEN)+30);
    this->setWindowOpacity(0.9);

    timer=new QTimer(this);
    hideBtn=new QPushButton(tr("隐藏"));
    fixedBtn =new QPushButton(tr("固定"));
    pictureBtn=new QPushButton(tr("加入图片"));
    updatecomboxBtn=new QPushButton(tr("更新目录"));
    uploadBtn=new QPushButton(tr("上传"));
    downloadBtn=new QPushButton(tr("下载"));
    tEdit=new QPlainTextEdit;
    combox=new QComboBox;
    clcbox=new QComboBox;
    clledit=new QLineEdit;
    clledit->hide();
    pictureLabel1=new QLabel;
    textcount=new QLabel("当前字数:");
    mainlayout=new QGridLayout(this);

    QStringList textlist;
    m_IniFile->beginGroup("text");
    textlist=m_IniFile->childKeys();
    for(int i=0;i<textlist.count();i++)
        clcbox->addItem(textlist.at(i));
    m_IniFile->endGroup();
    clcbox->addItem("添加新文本");
    textstr=m_IniFile->value("text/"+textlist[0]).toString();
    preindex=0;

    hideBtn->setStyleSheet("background-color:white");
    fixedBtn->setStyleSheet("background-color:white");
    pictureBtn->setStyleSheet("background-color:white");
    updatecomboxBtn->setStyleSheet("background-color:white");
    uploadBtn->setStyleSheet("background-color:white");
    downloadBtn->setStyleSheet("background-color:white");
    clcbox->setStyleSheet("background-color:white");
    clledit->setStyleSheet("background-color:white");
    textcount->setStyleSheet("color:white");
    tEdit->setStyleSheet("background-image:url(:/new/prefix1/syoujyo.png)");
    combox->setStyleSheet("background-color:yellow");

    mainlayout->addWidget(fixedBtn,0,0);
    mainlayout->addWidget(hideBtn,0,1);
    mainlayout->addWidget(clcbox,1,0,1,2);
    mainlayout->addWidget(clledit,1,0,1,2);
    mainlayout->addWidget(uploadBtn,2,0,1,2,Qt::AlignLeft);
    mainlayout->addWidget(downloadBtn,2,0,1,2,Qt::AlignCenter);
    mainlayout->addWidget(textcount,2,0,1,2,Qt::AlignRight);
    mainlayout->addWidget(tEdit,3,0,1,2);
    mainlayout->addWidget(pictureBtn,4,0);
    mainlayout->addWidget(updatecomboxBtn,4,1);
    mainlayout->addWidget(combox,5,0,1,2);
    mainlayout->addWidget(pictureLabel1,6,0,1,2,Qt::AlignCenter);
    mainlayout->setSpacing(5);

    pictureLabel1->setMinimumSize(490,450);

    path=QCoreApplication::applicationDirPath();
    path+="/AppDate/";
    dir.setPath(path);
    if (!dir.exists(path))
    dir.mkpath(path);
    file_list = dir.entryInfoList(QDir::Files | QDir::Hidden);
        for(int i=0;i<file_list.count();i++)
        {
            filenamelist.append(file_list.at(i).fileName());
            combox->addItem(file_list.at(i).fileName());
        }
        m_IniFile->setValue("picture/count",file_list.count());
        qDebug()<<file_list;
        for(int m=0;m<file_list.count();m++)
        {
            if(path+file_list.at(m).fileName()==m_IniFile->value("picture/current").toString())
               combox->setCurrentIndex(m);
        }
    tEdit->setFixedSize(470,500);
    tEdit->setFont(QFont(tr("宋体"), 11,50));
    tEdit->setCursorWidth(2);


    cursor = tEdit->textCursor();
    cursor.mergeCharFormat(fmt);
    tEdit->mergeCurrentCharFormat(fmt);
    tEdit->setPlainText(textstr);
    tEdit->setContextMenuPolicy(Qt::CustomContextMenu);
    createMenuandAction();
    firstrun=true;

    QPixmap *img= new QPixmap(m_IniFile->value("picture/current").toString());
    QPixmap fitpixmap =img->scaled(470, 450, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    pictureLabel1->setPixmap(fitpixmap);

    pictureLabel1->installEventFilter(this);
    clcbox->installEventFilter(this);


    SetWindowLong((HWND)winId(),GWL_EXSTYLE,WS_EX_TOOLWINDOW);
    prestrcount=tEdit->toPlainText().length();

    connect(fixedBtn,SIGNAL(clicked(bool)),this,SLOT(fixedpro()));
    connect(hideBtn,SIGNAL(clicked(bool)),this,SLOT(hidepro()));
    connect(pictureBtn,SIGNAL(clicked(bool)),this,SLOT(picturepro()));
    connect(updatecomboxBtn,SIGNAL(clicked(bool)),this,SLOT(updatepro()));
    connect(tEdit,SIGNAL(cursorPositionChanged()),this,SLOT(cursorchangepro()));
    connect(tEdit,SIGNAL(textChanged()),this,SLOT(textchangepro()));
    connect(timer,SIGNAL(timeout()),this,SLOT(timepro()));
    connect(combox,SIGNAL(activated(int)),this,SLOT(comboxpro(int)));
    connect(tEdit,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(showMenu(QPoint)));
    connect(clledit,SIGNAL(returnPressed()),this,SLOT(showcombox()));
    connect(clcbox,SIGNAL(activated(QString)),this,SLOT(showtextandadd(QString)));
    connect(uploadBtn,SIGNAL(clicked(bool)),this,SLOT(uploadpro()));
    connect(downloadBtn,SIGNAL(clicked(bool)),this,SLOT(downloadpro()));
}
void sidewidget::textchangepro()
{
    cursor.mergeCharFormat(fmt);
    tEdit->mergeCurrentCharFormat(fmt);
    textstr=tEdit->toPlainText();
    textcount->setText("当前字数："+QString::number(tEdit->toPlainText().length(),10));
}
void sidewidget::uploadpro()
{
    timer->stop();
    QMessageBox mesbox(QMessageBox::NoIcon, "上传", "你想分享你的文档到服务器上吗", QMessageBox::Yes | QMessageBox::No, NULL);
    QSettings *dataIni = new QSettings(QApplication::applicationDirPath()+"/data.ini", QSettings::IniFormat);
    dataIni->beginGroup("text");
    QStringList strlist=dataIni->childKeys();
    dataIni->endGroup();
    if(mesbox.exec()==QMessageBox::Yes)
    {
        if(strlist.contains(clcbox->currentText()))
        {
            QMessageBox::information(0,"上传失败","存在相同文本",0);
            timer->start(100);
        }
        else
        {
        dataIni->setValue("text/"+clcbox->currentText(),tEdit->toPlainText());
        QMessageBox::information(0,"上传成功","上传成功",0);
        timer->start(100);
        }
    }
    else
    timer->start(100);
}
void sidewidget::downloadpro()
{
    timer->stop();
    wid=new downloadWidget;
    connect(wid,SIGNAL(addsignal(QString,QString)),this,SLOT(dodownloadpro(QString,QString)));
    wid->exec();
    timer->start(200);
}
void sidewidget::dodownloadpro(QString title, QString text)
{
    for(int i=0;i<clcbox->count()-1;i++)
    {
        if(clcbox->itemText(i)==title)
        {
            QMessageBox::information(0,"失败","存在相同项",0);
            return;
        }
    }
    m_IniFile->setValue("text/"+title,text);
    clcbox->insertItem(0,title);
    preindex++;
     QMessageBox::information(0,"成功","添加成功",0);
}
void sidewidget::showtextandadd(QString str)
{
    int index=clcbox->currentIndex();
    opflag=false;
    m_IniFile->setValue("text/"+clcbox->itemText(preindex),tEdit->toPlainText()); //保存上一个文本
    preindex=index;
    if(index!=clcbox->count()-1)
    {
        tEdit->setPlainText(m_IniFile->value("text/"+str).toString());
    }
    else//增添操作
    {
        opflag=true;
        clledit->setText("请输入名称");
        clcbox->hide();
        clledit->show();
        clledit->setFocus();
    }

}
void sidewidget::showcombox()
{
    int index=clcbox->currentIndex();
    if(index!=clcbox->count()-1)
    {
    if(clledit->text()!="")//改名操作
    {
    m_IniFile->setValue("text/"+clledit->text(),tEdit->toPlainText());//保存当前值
    m_IniFile->remove("text/"+clcbox->currentText());//移除先前值
    //更新列表框
    clcbox->removeItem(index);
    clcbox->insertItem(index,clledit->text());
    clcbox->setCurrentIndex(index);
    }
    if(clledit->text()=="")//删除操作
    {
        if(clcbox->currentIndex()==clcbox->count()-2)//为倒数第二个
        {
            tEdit->clear();
            m_IniFile->remove("text/"+clcbox->currentText());//移除项
            clcbox->removeItem(index);
            clcbox->setCurrentIndex(clcbox->count()-2);
            tEdit->setPlainText(m_IniFile->value("text/"+clcbox->currentText()).toString());

        }
        else
        {
        tEdit->clear();
        m_IniFile->remove("text/"+clcbox->currentText());//移除项
        clcbox->removeItem(index);
        tEdit->setPlainText(m_IniFile->value("text/"+clcbox->currentText()).toString());
        }
    }
    }
    else//增添操作
    {
        if(clledit->text()=="")//创建为空则不创建
        {
            clledit->hide();
            clcbox->show();
            opflag=false;
            return;
        }
        m_IniFile->setValue("text/"+clledit->text(),"");//保存当前值
        tEdit->clear();
        clcbox->insertItem(index,clledit->text());
        clcbox->setCurrentIndex(index);
    }
    clledit->hide();
    clcbox->show();
    opflag=false;
}
void sidewidget::hidepro()
{
    double i;
    if(anime)
    {
    for(i=0;i>-500;i-=0.0002)
    {
        move((int)i,0);
    }
    i=0;
    }
    m_IniFile->setValue("text/"+clcbox->currentText(),tEdit->toPlainText());
    this->hide();
    this->move(0,0);
    sideshowed=false;
    timer->stop();
}
void sidewidget::fixedpro()
{
    if(sidefixed==false)
    {
        timer->stop();
        fixedBtn->setText(tr("取消固定"));
        sidefixed=true;
        return;
    }
    if(sidefixed==true)
    {
        timer->start(100);
        fixedBtn->setText(tr("固定"));
        sidefixed=false;
        return;
    }

}
void sidewidget::cursorchangepro()
{
    static bool in=false;
    static int pt;
    if(tEdit->toPlainText().length()>prestrcount)//表示键入
    {
    QTextCursor cur=tEdit->textCursor();
    int p=cur.position();
    QString c=tEdit->toPlainText().mid(--p,1);
    if(c=="(")
    {
        tEdit->blockSignals(true);
        cur.insertText(")");
        cur.setPosition(++p);
        tEdit->setTextCursor(cur);
        tEdit->blockSignals(false);
    }
    else if(c=="[")
    {
        tEdit->blockSignals(true);
        cur.insertText("]");
        cur.setPosition(++p);
        tEdit->setTextCursor(cur);
        tEdit->blockSignals(false);
    }
    else if(c=="\"")
    {
        tEdit->blockSignals(true);
        cur.insertText("\"");
        cur.setPosition(++p);
        tEdit->setTextCursor(cur);
        tEdit->blockSignals(false);
    }
    else if(c=="'")
    {
        tEdit->blockSignals(true);
        cur.insertText("'");
        cur.setPosition(++p);
        tEdit->setTextCursor(cur);
        tEdit->blockSignals(false);
    }
    else if(c=="{")
    {
        tEdit->blockSignals(true);
        cur.insertText("}");
        cur.setPosition(++p);
        tEdit->setTextCursor(cur);
        tEdit->blockSignals(false);
        pt=cur.position();//保存花括号坐标
        in=true;
    }
    else if(in && c=="\n" && pt==p)
    {
        tEdit->blockSignals(true);
        cur.insertText("\n");
        cur.setPosition(++p);
        tEdit->setTextCursor(cur);
        cur.insertText("    ");
        tEdit->blockSignals(false);
        in=false;
    }
    }
    prestrcount=tEdit->toPlainText().length();//保存字数
}
void sidewidget::timepro()
{
    POINT p;
    GetCursorPos(&p);
    if(p.x>500 && !sidefixed)
        hidepro();
}
void sidewidget::picturepro()
{
            timer->stop();
            QFileDialog *fileDialog = new QFileDialog(this);
            fileDialog->setWindowTitle(tr("打开图片"));
            fileDialog->setDirectory(".");
            fileDialog->setNameFilter(tr("Images(*.png *.jpg *.jpeg *.bmp)"));
            fileDialog->setFileMode(QFileDialog::ExistingFiles);
            fileDialog->setViewMode(QFileDialog::Detail);
            QStringList fileNames;
            if(fileDialog->exec())
            {
                fileNames = fileDialog->selectedFiles();
                QString temp=fileNames[0];
                QStringList namelist = temp.split("/");//分割后的名字
                QFile::copy(temp, path+namelist.at(namelist.count()-1));
                if(filenamelist.count()==0)
                {
                    combox->blockSignals(true);
                    combox->clear();
                    combox->addItem(namelist[namelist.count()-1]);
                    combox->blockSignals(false);
                    filenamelist.append(namelist.at(namelist.count()-1));
                    combox->setCurrentIndex(m_IniFile->value("picture/count").toInt());
                    comboxpro(combox->currentIndex());
                    m_IniFile->setValue("picture/count",m_IniFile->value("picture/count").toInt()+1);

                }
                else
                {
                    combox->addItem(namelist[namelist.count()-1]);
                    filenamelist.append(namelist.at(namelist.count()-1));
                    combox->setCurrentIndex(m_IniFile->value("picture/count").toInt());
                    comboxpro(combox->currentIndex());
                    m_IniFile->setValue("picture/count",m_IniFile->value("picture/count").toInt()+1);
                }

            }
            timer->start(100);
}
void sidewidget::updatepro()
{
    combox->blockSignals(true);
    combox->clear();

    //调用初始化方法
    filenamelist.clear();
    file_list = dir.entryInfoList(QDir::Files | QDir::Hidden);
    if(file_list.count()!=0)//循环寻找并加入下拉框
    {
        for(int i=0;i<file_list.count();i++)
        {
            filenamelist.append(file_list.at(i).fileName());
            combox->addItem(file_list.at(i).fileName());
        }
        m_IniFile->setValue("picture/count",file_list.count());
        combox->blockSignals(false);
        for(int m=0;m<file_list.count();m++)
        {
            if(path+file_list.at(m).fileName()==m_IniFile->value("picture/current").toString())
               combox->setCurrentIndex(m);
        }
    }

    tEdit->setFont(QFont(tr("宋体"), 11,50));


    cursor = tEdit->textCursor();
    cursor.mergeCharFormat(fmt);
    tEdit->mergeCurrentCharFormat(fmt);
    tEdit->setPlainText(textstr);

}
void sidewidget::comboxpro(int index)
{
    qDebug()<<index;
    QPixmap *img= new QPixmap(path+"/"+filenamelist.at(index));
    QPixmap fitpixmap =img->scaled(470, 450, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    if(!fitpixmap.isNull())
    pictureLabel1->setPixmap(fitpixmap);
    else
        pictureLabel1->setText(filenamelist.at(index));
    m_IniFile->setValue("picture/current",path+filenamelist.at(index));
}
bool sidewidget::eventFilter(QObject *obj, QEvent *event)
{
    if(obj==pictureLabel1)
    {
        if (event->type() == QEvent::MouseButtonPress)//为标签添加点击事件
        {
            QString str=m_IniFile->value("picture/current").toString();
            qDebug()<<str;
            str.replace("/","\\");
            ShellExecute(NULL, L"open", L"explorer.exe",str.toStdWString().c_str(), NULL, SW_SHOWNORMAL);
        }
    }
    if(obj==clcbox)
    {

        if(event->type()==QEvent::ContextMenu)//右键启动输入框
        {
            opflag=true;//正在操作
            clledit->setText(clcbox->currentText());
            clcbox->hide();
            clledit->show();
            clledit->setFocus();
        }
    }
}
void sidewidget::savepro()
{
    if(!opflag)
    m_IniFile->setValue("text/"+clcbox->currentText(),tEdit->toPlainText());
}
void sidewidget::showMenu(QPoint p)
{
    if(tEdit->textCursor().selectedText()=="")
        {
        copyAction->setDisabled(true);
        cutAction->setDisabled(true);
        URLAction->setDisabled(true);
        }
    else
    {
        copyAction->setDisabled(false);
        cutAction->setDisabled(false);
        URLAction->setDisabled(false);
    }
    //selectallAction->setDisabled(true);
    textMenu->move(p);
    textMenu->show();
}
void sidewidget::createMenuandAction()
{
    textMenu=new QMenu(this);
    cutAction=new QAction(tr("剪切"),this);
    copyAction=new QAction(tr("复制"),this);
    selectallAction=new QAction(tr("全选"),this);
    URLAction=new QAction(tr("打开URL"),this);
    pasteAction=new QAction(tr("粘贴"),this);
    textMenu->addAction(copyAction);
    textMenu->addAction(cutAction);
    textMenu->addAction(selectallAction);
    textMenu->addAction(pasteAction);
    textMenu->addSeparator();
    textMenu->addAction(URLAction);

    connect(copyAction,SIGNAL(triggered(bool)),SLOT(copypro()));
    connect(cutAction,SIGNAL(triggered(bool)),SLOT(cutpro()));
    connect(selectallAction,SIGNAL(triggered(bool)),SLOT(selectallypro()));
    connect(URLAction,SIGNAL(triggered(bool)),SLOT(URLpro()));
    connect(pasteAction,SIGNAL(triggered(bool)),SLOT(pastepro()));
}
void sidewidget::copypro()//复制
{

    QTextCursor cur=tEdit->textCursor();
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(cur.selectedText());
}
void sidewidget::cutpro()//复切
{
    QTextCursor cur=tEdit->textCursor();
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(cur.selectedText());
    cur.removeSelectedText();
}
void sidewidget::pastepro()//粘贴
{
    QTextCursor cur=tEdit->textCursor();
    QClipboard *clipboard = QApplication::clipboard();
    if(cur.selectedText()!="")
    {
        cur.removeSelectedText();
        cur.insertText(clipboard->text());
    }
    else
    cur.insertText(clipboard->text());
}
void sidewidget::selectallypro()//全选
{
    tEdit->moveCursor(QTextCursor::Start,QTextCursor::MoveMode::MoveAnchor);
    tEdit->moveCursor(QTextCursor::End,QTextCursor::MoveMode::KeepAnchor);
}
void sidewidget::URLpro()//打开指定网页
{
    QString temp=tEdit->textCursor().selectedText();
    QDesktopServices::openUrl(QUrl(temp));
}
