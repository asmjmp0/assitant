#ifndef OPTION_H
#define OPTION_H

#include <QDialog>
#include<QPushButton>
#include<QGridLayout>
#include<QLabel>
#include<QCheckBox>;
#include<QSettings>
#include<QLabel>
#include<QLineEdit>
#include<QSlider>
#include<QColorDialog>
#include<QPushButton>
#include<QComboBox>
class option : public QWidget
{
    Q_OBJECT
public:
    explicit option(QWidget *parent = nullptr);

private:
    QPushButton *apply;
    QPushButton *closeBtn;
    QGridLayout *mainlayout;
    QLabel *label;
    QCheckBox *cbox;
    QCheckBox *sidebox;
    QCheckBox *animebox;
    QSlider *slider;
    QSettings *preg;
    QLabel *timeLabel;
    QLabel *OpacityLabel;
    QLineEdit *timeLineedit;
    QComboBox *colorselect;
public slots:
    void applyfun();
    void closefun();
    void sliderpro(int);
    void sliderleave();
    void colorselected();
signals:
    void setOpacity(int);
    void itleaveslider();
    void fontcolor(QString);
};

#endif // OPTION_H
