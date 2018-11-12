#ifndef SIDEWIDGET_H
#define SIDEWIDGET_H

#include <QWidget>
#include<windows.h>
#include<QPushButton>
#include<QGridLayout>
#include<QPlainTextEdit>
#include<QLabel>
#include<QSettings>
#include<QTimer>
#include<QFileDialog>
#include<QComboBox>
#include<QEvent>
#include<QObject>
#include<QTextCursor>
#include<QDir>
#include<QFileInfoList>
#include<QFile>
#include<QMenu>
#include<QAction>
#include<QPoint>
#include<QClipboard>
#include<QLineEdit>
#include"downloadwidget.h"
class sidewidget : public QWidget
{
    Q_OBJECT
public:
    explicit sidewidget(QWidget *parent = nullptr);
    QTimer *timer;
    QTextCharFormat fmt;
    QComboBox *clcbox;
    QPlainTextEdit   *tEdit;
private:
    QPushButton *hideBtn;
    QPushButton *fixedBtn;
    QPushButton *pictureBtn;
    QPushButton *updatecomboxBtn;
    QPushButton *uploadBtn;
    QPushButton *downloadBtn;
    QLabel *textcount;
    QComboBox *combox;
    QLabel *pictureLabel1;
    QGridLayout *mainlayout;
    QTextCursor cursor;
    QDir dir;
    QString path;
    QFileInfoList file_list;
    QStringList filenamelist;
    QMenu *textMenu;
    QAction *cutAction;
    QAction *copyAction;
    QAction *selectallAction;
    QAction *pasteAction;
    QAction *URLAction;
    QLineEdit *clledit;
    downloadWidget *wid;
    int prestrcount;
    bool firstrun;
    void createMenuandAction();
    int preindex;
public slots:
    void updatepro();
    void hidepro();
private slots:
    void fixedpro();
    void textchangepro();
    void cursorchangepro();
    void timepro();
    void picturepro();
    void savepro();
    void comboxpro(int);
    void showMenu(QPoint);

    void copypro();
    void cutpro();
    void pastepro();
    void selectallypro();
    void URLpro();
    void showcombox();
    void showtextandadd(QString);
    void uploadpro();
    void downloadpro();
    void dodownloadpro(QString,QString);
protected:
    bool eventFilter(QObject *obj, QEvent *event);
};

#endif // SIDEWIDGET_H
