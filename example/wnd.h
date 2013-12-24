#ifndef WND_H
#define WND_H

#include <QWidget>
#include <QListWidget>
#include <QComboBox>
#include "navbar.h"

class Wnd : public QWidget
{
    Q_OBJECT
    
public:
    Wnd(QWidget *parent = 0);
    ~Wnd();

private slots:
    void changeStylesheet(const QString &styleName);
    void navBarCurrentChanged(int index);
    void navBarVisibleRowsChanged(int rows);

protected:
    NavBar *navBar;
    QListWidget *signalWidget;
    QComboBox *styleBox;
};

#endif // WND_H
