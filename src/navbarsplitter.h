#ifndef NAVBARSPLITTER_H
#define NAVBARSPLITTER_H

#include <QSplitter>

class NavBarSplitter : public QSplitter
{
    Q_OBJECT
    Q_PROPERTY(int increment READ increment WRITE setIncrement)

public:
    explicit NavBarSplitter(QWidget *parent = 0);

    int increment() const;
    void setIncrement(int inc);

protected:
    QSplitterHandle *createHandle();
};

class NavBarSplitterHandle : public QSplitterHandle
{
    Q_OBJECT

public:
    explicit NavBarSplitterHandle(Qt::Orientation orientation, QSplitter *parent);

    int increment;

protected:
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);

private:
    int  mouseOffset;
    bool pressed;
};

#endif // NAVBARSPLITTER_H
