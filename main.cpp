#include "assistant.h"
#include <QApplication>
extern QString name;
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    name=argv[0];
    assistant w;
    return a.exec();
}
