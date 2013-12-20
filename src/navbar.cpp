#include <QWidget>
#include <QStackedWidget>
#include <QToolBar>
#include <QVBoxLayout>
#include <QList>
#include <QAction>
#include <QActionGroup>
#include <QResizeEvent>
#include <QAction>
#include <QList>
#include "navbar.h"

/**
 * @class NavBar
 * @brief Outlook-like navigation bar.
 *
 * Description...
 */
/**
 * @property NavBar::count
 * Number of pages in navigation bar.
 */
/**
 * @property NavBar::currentIndex
 * This property holds the index of the current page.
 */
/**
 * @property NavBar::rowHeight
 * This property holds height of a row in a page list.
 */
/**
 * @property NavBar::visibleRows
 * This property holds number of visible rows.
 * It's value changed when splitter is moved.
 */
/**
 * @fn NavBar::currentChanged
 * This signal is emitted when the current item is changed.
 * @param index Index of selected page
 */
/**
 * @fn NavBar::visibleRowsChanged
 * This signal is emitted when number of visible rows in navigation bar page list changed, e.g. when splitter is moved.
 * @param rows Number of visible rows
 **/

/**
 * Constructs new NavBar
 * @param parent Parent widget
 */
NavBar::NavBar(QWidget *parent):
    QFrame(parent)
{
    stackedWidget = new QStackedWidget(this);
    pageList = new NavBarPageList(this);
    pageToolBar = new QToolBar(this);  
    pageToolBar->setMinimumHeight(pageList->rowHeight());

    splitter = new NavBarSplitter(this);
    splitter->setOrientation(Qt::Vertical);
    splitter->addWidget(stackedWidget);
    splitter->addWidget(pageList);
    splitter->setStretchFactor(0, 1);
    splitter->setStretchFactor(1, 0);

    int left, top, right, bottom;
    int rh = pageList->rowHeight();
    getContentsMargins(&left, &top, &right, &bottom);
    splitter->setGeometry(left, top, width()-right-left, height()-(rh+top+bottom));
    pageToolBar->setGeometry(left, height()-(rh+bottom), width()-left-right, rh);

    actionGroup = new QActionGroup(this);
    actionGroup->setExclusive(true);

    connect(actionGroup, SIGNAL(triggered(QAction*)), SLOT(onClickPageButton(QAction*)));
    connect(pageList, SIGNAL(buttonVisibilityChanged(int)), SLOT(onButtonVisibilityChanged(int)));
}

NavBar::~NavBar()
{
    for(int i = 0; i < pageActions.size(); i++)
        delete pageActions[i];

    pageActions.clear();
}

int NavBar::count() const
{
    return stackedWidget->count();
}

int NavBar::currentIndex() const
{
    return stackedWidget->currentIndex();
}

int NavBar::rowHeight() const
{
    return pageList->rowHeight();
}

void NavBar::setRowHeight(int height)
{
    int rows = visibleRows();

    pageList->setRowHeight(height);
    pageToolBar->setMinimumHeight(height);
    splitter->setIncrement(height);
    setVisibleRows(rows);
}

int NavBar::visibleRows() const
{
    return pageList->height() / rowHeight();
}

void NavBar::setVisibleRows(int rows)
{
    int left, top, right, bottom;
    getContentsMargins(&left, &top, &right, &bottom);
    pageList->resize(width()-right-left, rows*rowHeight());
}

void NavBar::resizeEvent(QResizeEvent *e)
{
    int rh = rowHeight();
    int left, top, right, bottom;
    getContentsMargins(&left, &top, &right, &bottom);

    splitter->setGeometry(left, top, e->size().width()-right-left, e->size().height()-(rh+top+bottom));
    pageToolBar->setGeometry(left, e->size().height()-(rh+bottom), e->size().width()-left-right, rh);

    QFrame::resizeEvent(e);
}

/*
QSize NavBar::smallIconSize() const
{
    return pageToolBar->iconSize();
}

void NavBar::setSmallIconSize(const QSize &size)
{
    pageToolBar->setIconSize(size);
    pageToolBar->setMinimumHeight(pageList->rowHeight());
}

QSize NavBar::largeIconSize() const
{
    return pageList->iconSize();
}

void NavBar::setLargeIconSize(const QSize &size)
{
    pageList->setIconSize(size);
}
*/

/**
 * Adds widget as new page to navigation bar
 * @param page Widget to be added as new page
 * @return The new page's index
 */
int NavBar::addPage(QWidget *page)
{
    return addPage(page, "");
}

/**
 * Adds widget as new page to navigation bar
 * @param page Widget to be added as new page
 * @param title Page title
 * @return The new page's index
 */
int NavBar::addPage(QWidget *page, const QString &title)
{
    return addPage(page, title, QIcon());
}

/**
 * Adds widget as new page to navigation bar
 * @param page Widget to be added as new page
 * @param title Page title
 * @param icon Page icon
 * @return The new page's index
 */
int NavBar::addPage(QWidget *page, const QString &title, const QIcon &icon)
{
    QAction *action = new QAction(this);
    action->setCheckable(true);
    action->setData(pageActions.size());
    action->setText(title);
    action->setIcon(icon);

    actionGroup->addAction(action);
    pageList->addItem(action);
    int idx = stackedWidget->addWidget(page);

    pageActions.append(action);
    pageActions[stackedWidget->currentIndex()]->setChecked(true);

    return idx;
}

/**
 * Inserts new page at given position, or at the bottom of the toolbox if index is out of range.
 * @param index Page position
 * @param page Widget
 * @return The new page's index
 */
int NavBar::insertPage(int index, QWidget *page)
{
    return insertPage(index, page, "");
}

/**
 * Inserts new page at given position, or at the bottom of the toolbox if index is out of range.
 * @param index Page position
 * @param page Widget
 * @param title Page title
 * @return The new page's index
 */
int NavBar::insertPage(int index, QWidget *page, const QString &title)
{
    return insertPage(index, page, title, QIcon());
}

/**
 * Inserts new page at given position, or at the bottom of the toolbox if index is out of range.
 * @param index Page position
 * @param page Widget
 * @param title Page title
 * @param icon Page icon
 * @return The new page's index
 */
int NavBar::insertPage(int index, QWidget *page, const QString &title, const QIcon &icon)
{
    QAction *action = new QAction(this);
    action->setCheckable(true);
    action->setData(pageActions.size());
    action->setText(title);
    action->setIcon(icon);

    actionGroup->addAction(action);
    pageList->insertItem(index, action);
    int idx = stackedWidget->insertWidget(index, page);

    pageActions.insert(index, action);
    pageActions[stackedWidget->currentIndex()]->setChecked(true);

    return idx;
}

/**
 * Removes widget from the NavBar. i.e., widget is not deleted but simply removed from the navigation bar, causing it to be hidden.
 * @param index Index of widget to be removed
 */
void NavBar::removePage(int index)
{
    stackedWidget->removeWidget(stackedWidget->widget(index));
    pageList->removeItem(index);
    actionGroup->removeAction(pageActions[index]);
    delete pageActions[index];
    pageActions.removeAt(index);
}

/**
 * Set page title.
 * @param index Index of page
 * @param title New title
 */
void NavBar::setPageTitle(int index, const QString &title)
{
    pageActions[index]->setText(title);
}

/**
 * Set page icon.
 * @param index Index of page
 * @param icon New icon
 */
void NavBar::setPageIcon(int index, const QIcon &icon)
{
    pageActions[index]->setIcon(icon);
}

/**
 * Returns the widget at the given index, or 0 if there is no such widget.
 * @param index Widget index
 * @return Widget
 */
QWidget *NavBar::widget(int index) const
{
    return stackedWidget->widget(index);
}

QSize NavBar::sizeHint() const
{
    return QSize(180, 300);
}

void NavBar::setCurrentIndex(int index)
{
    stackedWidget->setCurrentIndex(index);
    pageActions[index]->setChecked(true);
}

void NavBar::onClickPageButton(QAction *action)
{
    int index = action->data().toInt();
    stackedWidget->setCurrentIndex(index);
    emit currentChanged(index);
}

void NavBar::onButtonVisibilityChanged(int visCount)
{
    pageToolBar->clear();
    QWidget *spacerWidget = new QWidget(this);
    spacerWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    spacerWidget->setVisible(true);
    pageToolBar->addWidget(spacerWidget);

    for(int i = 0; i < pageActions.size(); i++)
        if(i > visCount-1)
            pageToolBar->addAction(pageActions[i]);

    emit visibleRowsChanged(visCount);
}
