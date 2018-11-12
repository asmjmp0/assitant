#ifndef VIEW_H
#define VIEW_H

#include <QDialog>
#include<QObject>
#include<QTextEdit>
#include<QVBoxLayout>
#include<QTimer>
#include<QCloseEvent>
class view : public QDialog
{
    Q_OBJECT
public:
    explicit view(QDialog *parent = nullptr);
    ~view();
private:
    QTextEdit *edit;
    QVBoxLayout *mainlayout;
    QTimer *timer1;
private slots:
    void onTimeout();
protected:
    void closeEvent(QCloseEvent *event);
};

#endif // VIEW_H
