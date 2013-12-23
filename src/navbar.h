#ifndef NAVBAR_H
#define NAVBAR_H

#include <QStackedWidget>
#include <QToolBar>
#include <QVBoxLayout>
#include <QList>
#include <QActionGroup>
#include "navbarsplitter.h"
#include "navbarpagelist.h"

class NavBar : public QFrame
{
    Q_OBJECT
    Q_PROPERTY(int count READ count)
    Q_PROPERTY(int currentIndex READ currentIndex WRITE setCurrentIndex NOTIFY currentChanged)
    Q_PROPERTY(int rowHeight READ rowHeight WRITE setRowHeight)
    Q_PROPERTY(int visibleRows READ visibleRows WRITE setVisibleRows NOTIFY visibleRowsChanged)
//  Q_PROPERTY(QSize smallIconsSize READ smallIconSize WRITE setSmallIconSize)
//  Q_PROPERTY(QSize largeIconsSize READ largeIconSize WRITE setLargeIconSize)

public:
    explicit NavBar(QWidget *parent = 0);
    ~NavBar();

    int  count() const;
    int  currentIndex() const;
    int  rowHeight() const;
    void setRowHeight(int height);
    int  visibleRows() const;
/*  QSize smallIconSize() const;
    void  setSmallIconSize(const QSize &size);
    QSize largeIconSize() const;
    void  setLargeIconSize(const QSize &size); */
    int  addPage(QWidget *page);
    int  addPage(QWidget *page, const QString &title);
    int  addPage(QWidget *page, const QString &title, const QIcon &icon);
    int  insertPage(int index, QWidget *page);
    int  insertPage(int index, QWidget *page, const QString &title);
    int  insertPage(int index, QWidget *page, const QString &title, const QIcon &icon);
    void removePage(int index);
    void setPageTitle(int index, const QString &title);
    void setPageIcon(int index, const QIcon &icon);
    QWidget *widget(int index) const;
    QSize sizeHint() const;

    static QString loadStyle(const QString &filename);

signals:
    void currentChanged(int index);
    void visibleRowsChanged(int rows);

public slots:
    void setCurrentIndex(int index);
    void setVisibleRows(int rows);

protected:
    void resizeEvent(QResizeEvent *e);

private slots:
    void onClickPageButton(QAction *action);
    void onButtonVisibilityChanged(int visCount);

private:
    void resizeContent(const QSize &size, int rowheight);

    QStackedWidget  *stackedWidget;
    NavBarSplitter  *splitter;
    NavBarPageList  *pageList;
    QToolBar        *pageToolBar;
    QList<QAction *> pageActions;
    QActionGroup    *actionGroup;
};

#endif // NAVBAR_H
