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
#include <QFile>
#include <QTextStream>
#include "navbar.h"

/**
 * @class NavBar
 * @brief Outlook-like navigation bar.
 *
 * @image html navbar.png
 * Description...
 * @code
   NavBar *navBar = new NavBar(this);
   navBar->setStyleSheet(NavBar::loadStyle(":/styles/office2003gray.css"));
   navBar->addPage(new QLabel("Page 1 contents"), "Page 1", QIcon(":/images/mail.png"));
   navBar->addPage(new QLabel("Page 2 contents"), "Page 2", QIcon(":/images/calendar.png"));
   @endcode
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
 * This property holds number of currently visible rows.
 * It's value changed when splitter is moved.
 */
/**
 * @property NavBar::showHeader
 * This property controls visibility of NavBar header.
 */
/**
 * @property NavBar::smallIconSize
 * This property holds size of icons in the bottom toolbar.
 */
/**
 * @property NavBar::largeIconSize
 * This property holds size of icons in the page list.
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
    headerVisible = true;
    headerHeight  = 26;
    setFrameStyle(QFrame::Panel | QFrame::Sunken);

    header = new NavBarHeader(this);
    header->setFrameStyle(QFrame::Panel | QFrame::Raised);
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

    resizeContent(size(), rowHeight());

    actionGroup = new QActionGroup(this);
    actionGroup->setExclusive(true);

    connect(actionGroup, SIGNAL(triggered(QAction*)),          SLOT(onClickPageButton(QAction*)));
    connect(pageList,    SIGNAL(buttonVisibilityChanged(int)), SLOT(onButtonVisibilityChanged(int)));
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

bool NavBar::showHeader() const
{
    return headerVisible;
}

void NavBar::setShowHeader(bool show)
{
    if(show != headerVisible)
    {
        headerVisible = show;
        header->setVisible(show);
        resizeContent(size(), rowHeight());
    }
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
    resizeContent(size(), height);
    setVisibleRows(rows);
}

int NavBar::visibleRows() const
{
    return pageList->height() / rowHeight();
}

void NavBar::setVisibleRows(int rows)
{
    int listHeight = rows * rowHeight();
    int pageHeight = splitter->height() - listHeight;
    QList<int> sizes;
    sizes.append(pageHeight);
    sizes.append(listHeight);
    splitter->setSizes(sizes);
}

void NavBar::resizeEvent(QResizeEvent *e)
{
    resizeContent(e->size(), rowHeight());
    QFrame::resizeEvent(e);
}

void NavBar::resizeContent(const QSize &size, int rowheight)
{
    int left, top, right, bottom;
    getContentsMargins(&left, &top, &right, &bottom);

    if(headerVisible)
    {
        header->setGeometry(left, top, size.width()-right-left, headerHeight);
        splitter->setGeometry(left, top + headerHeight, size.width()-right-left, size.height()-(rowheight+top+bottom+headerHeight));
    }
    else
        splitter->setGeometry(left, top, size.width()-right-left, size.height()-(rowheight+top+bottom));

    pageToolBar->setGeometry(left, size.height()-(rowheight+bottom), size.width()-left-right, rowheight);
}

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
    return createPage(-1, page, title, icon);
}

/**
 * Inserts new page at given position, or at the bottom of the navigation bar if index is out of range.
 * @param index Page position
 * @param page Widget
 * @return The new page's index
 */
int NavBar::insertPage(int index, QWidget *page)
{
    return insertPage(index, page, "");
}

/**
 * Inserts new page at given position, or at the bottom of the navigation bar if index is out of range.
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
 * Inserts new page at given position, or at the bottom of the navigation bar if index is out of range.
 * @param index Page position
 * @param page Widget
 * @param title Page title
 * @param icon Page icon
 * @return The new page's index
 */
int NavBar::insertPage(int index, QWidget *page, const QString &title, const QIcon &icon)
{
    return createPage(index, page, title, icon);
}

/**
 * @internal
 * Creates and inserts new page at given position. If index is -1, page wil be added.
 * @param index Page index
 * @param page Widget
 * @param title Page title
 * @param icon Page icon
 * @return The new page's index
 */
int NavBar::createPage(int index, QWidget *page, const QString &title, const QIcon &icon)
{
    QAction *action = new QAction(this);
    action->setCheckable(true);
    action->setText(title);
    action->setIcon(icon);
    actionGroup->addAction(action);

    int idx;
    if(index < 0) // add page
    {
        action->setData(pageActions.size());
        pageList->addItem(action);
        idx = stackedWidget->addWidget(page);
        pageActions.append(action);
    }
    else // insert page
    {
        pageList->insertItem(index, action);
        idx = stackedWidget->insertWidget(index, page);
        pageActions.insert(index, action);

        for(int i = 0; i < pageActions.size(); i++)
            pageActions[i]->setData(i);
    }

    pageActions[stackedWidget->currentIndex()]->setChecked(true);
    header->setText(pageActions[stackedWidget->currentIndex()]->text());

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
 * Returns the title of the page at given position, or an empty string if index is out of range.
 * @param index Page index
 * @return Page title
 */
QString NavBar::pageTitle(int index) const
{
    return pageActions[index]->text();
}

/**
 * Returns the icon of the page at given position, or a null icon if index is out of range.
 * @param index Page index
 * @return Page icon
 */
QIcon NavBar::pageIcon(int index) const
{
    return pageActions[index]->icon();
}

/**
 * Sets the title of the page at given position.
 * @param index Page index
 * @param title New title
 */
void NavBar::setPageTitle(int index, const QString &title)
{
    pageActions[index]->setText(title);
}

/**
 * Sets the icon of the page at given position.
 * @param index Page index
 * @param icon New icon
 */
void NavBar::setPageIcon(int index, const QIcon &icon)
{
    pageActions[index]->setIcon(icon);
}

/**
 * Returns the widget at given index, or 0 if there is no such widget.
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
    header->setText(pageActions[index]->text());
    pageActions[index]->setChecked(true);
    emit currentChanged(index);
}

void NavBar::onClickPageButton(QAction *action)
{
    int current = stackedWidget->currentIndex();
    int index = action->data().toInt();

    if(index != current)
    {
        stackedWidget->setCurrentIndex(index);
        header->setText(action->text());
        emit currentChanged(index);
    }
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

    //TODO: do not emit this, when rowHeight is changed.
    emit visibleRowsChanged(visCount);
}

/**
 * Helper function, which loads text file and returns it content as QString.
 * @param filename Style sheet file name
 * @return Style sheet
 */
QString NavBar::loadStyle(const QString &filename)
{
    QFile stylefile(filename);
    if(stylefile.open(QFile::ReadOnly))
    {
        QTextStream stream(&stylefile);
        QString stylesheet = stream.readAll();
        return stylesheet;
    }
    else
        return "";
}


NavBarHeader::NavBarHeader(QWidget *parent):
    QLabel(parent)
{
}
