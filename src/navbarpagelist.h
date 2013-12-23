#ifndef NAVBARPAGELIST_H
#define NAVBARPAGELIST_H

#include <QWidget>
#include <QList>
#include <QToolButton>

class NavBarPageList : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(int rowHeight READ rowHeight WRITE setRowHeight)
    //Q_PROPERTY(QSize iconSize READ iconSize WRITE setIconSize)

public:
    explicit NavBarPageList(QWidget *parent = 0);
    ~NavBarPageList();

    int   rowHeight() const;
    void  setRowHeight(int newHeight);
    //QSize iconSize() const;
    //void  setIconSize(const QSize &size);
    void  addItem(QAction *action);
    void  insertItem(int index, QAction *action);
    void  removeItem(int index);

signals:
    void buttonVisibilityChanged(int visCount);

protected:
    void resizeEvent(QResizeEvent *e);
    virtual QAbstractButton *createPageButton(QAction *action);

private:
    QList<QAbstractButton *> buttons;
    int                      pageButtonHeight;
};

#endif // NAVBARPAGELIST_H
