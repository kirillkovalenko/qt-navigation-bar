#include "wnd.h"
#include <QApplication>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Q_INIT_RESOURCE(navbar);

    //Loading NavBar translation (options dialog & menu)
    QString locale = QLocale::system().name();
    QTranslator translator;
    translator.load(QString("navigationbar_") + locale);
    app.installTranslator(&translator);

    Wnd wnd;
    wnd.show();
    
    return app.exec();
}
