#include "wnd.h"
#include <QApplication>
#include <QFile>
#include <QTextStream>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QFile stylefile(":/styles/2003gray.qss");
    if(stylefile.open(QFile::ReadOnly))
    {
        QTextStream stream(&stylefile);
        QString stylesheet = stream.readAll();
        app.setStyleSheet(stylesheet);
    }

    Wnd wnd;
    wnd.show();
    
    return app.exec();
}
