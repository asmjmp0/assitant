#include "option.h"
#include<QDebug>
#include<windows.h>
#include"assistant.h"
extern bool findit;
extern bool self;
extern bool moveit;
extern QString name;
extern bool sideenable;
extern bool sidefixed;
extern bool anime;
extern int savetime;
extern sidewidget *side;
extern QSettings *m_IniFile;
extern int sideOpacity;
option::option(QWidget *parent) : QWidget(parent)
{
    this->setWindowFlags(Qt::FramelessWindowHint);
    QString pathstr="HKEY_LOCAL_MACHINE\\Software\\Microsoft\\Windows\\CurrentVersion\\Run";
    preg=new QSettings(pathstr,QSettings::NativeFormat);
    //m_IniFile = new QSettings("option.ini", QSettings::IniFormat);
    apply=new QPushButton(tr("apply"));
    closeBtn=new QPushButton(tr("clsoe"));
    colorselect=new QComboBox;
    label=new QLabel(tr("功能逐渐完善中"));
    OpacityLabel=new QLabel("透明度:");
    cbox=new QCheckBox(tr("开机启动"));
    sidebox=new QCheckBox(tr("侧边栏"));
    animebox=new QCheckBox(tr("显示动画"));
    timeLabel=new QLabel(tr("存储间隔/min:"));
    slider=new QSlider(Qt::Horizontal);
    timeLineedit=new QLineEdit;
    timeLineedit->setText(QString::number(savetime,10));
    QStringList list;
    list=preg->childKeys();
    for(int i=0;i<list.count();i++)
    {
        if(list.at(i)=="assistant")
        {
            findit=true;
            break;
        }
        else
        {
            findit=false;
        }

    }
    cbox->setChecked(findit);
    sidebox->setChecked(sideenable);
    animebox->setChecked(anime);
    slider->setMinimum(0);
    slider->setMaximum(99);
    slider->setValue(sideOpacity);
    colorselect->addItem("white");
    colorselect->addItem("black");
    colorselect->addItem("red");
    colorselect->addItem("magenta");
    colorselect->addItem("yellow");
    colorselect->addItem("blue");
    colorselect->addItem("gray");
    colorselect->addItem("green");
    colorselect->addItem("lightGray");

    for(int i=0;i<9;i++)
    {
        colorselect->setCurrentIndex(i);
        if(m_IniFile->value("sidewidget/fontcolor")==colorselect->currentText())
            break;
    }

    mainlayout=new QGridLayout(this);
    mainlayout->addWidget(label,0,0,1,2);
    mainlayout->addWidget(cbox,1,0,1,2);
    mainlayout->addWidget(sidebox,2,0,1,2);
    mainlayout->addWidget(animebox,3,0,1,2);
    mainlayout->addWidget(timeLabel,4,0);
    mainlayout->addWidget(timeLineedit,4,1);
    mainlayout->addWidget(OpacityLabel,5,0);
    mainlayout->addWidget(slider,5,1);
    mainlayout->addWidget(colorselect,6,0,1,2);
    mainlayout->addWidget(apply,7,0);
    mainlayout->addWidget(closeBtn,7,1);

    connect(apply,SIGNAL(clicked(bool)),this,SLOT(applyfun()));
    connect(closeBtn,SIGNAL(clicked(bool)),this,SLOT(closefun()));
    connect(slider,SIGNAL(valueChanged(int)),this,SLOT(sliderpro(int)));
    connect(slider,SIGNAL(sliderReleased()),this,SLOT(sliderleave()));
    connect(colorselect,SIGNAL(currentIndexChanged(int)),this,SLOT(colorselected()));
}
void option::applyfun()
{

    if(!cbox->isChecked() && findit)
    {

        if(findit)
        {
        preg->remove("assistant");
        m_IniFile->setValue( "self/self",0);
        }
    }
    else if(!findit && cbox->isChecked())
    {
        preg->setValue("assistant","\""+name+"\"");
        m_IniFile->setValue( "self/self",1);
    }
    if(sidebox->isChecked())
    {
        m_IniFile->setValue("sidewidget/enable",1);
        sideenable=true;
    }
    if(!sidebox->isChecked())
    {
        m_IniFile->setValue("sidewidget/enable",0);
        sideenable=false;
    }
    if(animebox->isChecked())
    {
        m_IniFile->setValue("sidewidget/anime",1);
        anime=true;
    }
    if(!animebox->isChecked())
    {
        m_IniFile->setValue("sidewidget/anime",0);
        anime=false;
    }
    savetime=timeLineedit->text().toInt();
    m_IniFile->setValue("sidewidget/savatime",timeLineedit->text().toInt());
    m_IniFile->setValue("sidewidget/Opacity",sideOpacity);
    m_IniFile->setValue("sidewidget/fontcolor",colorselect->currentText());
    m_IniFile->setValue("sidewidget/anime",anime);
     hide();
}
void option::sliderpro(int value)
{
    emit setOpacity(value);
}
void option::sliderleave()
{
    emit itleaveslider();
}
void option::colorselected()
{
    emit fontcolor(colorselect->currentText());
}
void option::closefun()
{
    hide();
}
