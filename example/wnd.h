#ifndef WND_H
#define WND_H

#include <QWidget>
#include <QListWidget>
#include <QComboBox>
#include <QCheckBox>
#include "navbar.h"

class Wnd : public QWidget
{
    Q_OBJECT
    
public:
    Wnd(QWidget *parent = 0);
    ~Wnd();

private slots:
    void changeStylesheet(int index);
    void navBarCurrentChanged(int index);
    void navBarVisibleRowsChanged(int rows);

protected:
    NavBar *navBar;
    QListWidget *signalWidget;
    QComboBox *styleBox;
    QCheckBox *showHeaderBox;
};

#endif // WND_H
