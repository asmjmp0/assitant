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
class sidewidget : public QWidget
{
    Q_OBJECT
public:
    explicit sidewidget(QWidget *parent = nullptr);
    QTimer *timer;
    QTextCharFormat fmt;
private:
    QPushButton *hideBtn;
    QPushButton *fixedBtn;
    QPushButton *pictureBtn;
    QPushButton *updatecomboxBtn;
    QPlainTextEdit   *tEdit;
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
    bool firstrun;
    void createMenuandAction();
public slots:
    void updatepro();
    void hidepro();
private slots:
    void fixedpro();
    void textchangepro();
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
protected:
    bool eventFilter(QObject *obj, QEvent *event);
};

#endif // SIDEWIDGET_H
