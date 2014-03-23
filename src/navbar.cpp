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
 * @class NavBarHeader
 * @brief Navigation bar header.
 *
 * This class derived from QLabel to have different class name (for use with style sheets) and has no additional features.
 */

/**
 * @class NavBar
 * @brief Outlook-like navigation bar.
 *
 * @image html navbar.png
 * The NavBar class provides a stack of pages where only one page is visible at a time,
 * and page list in lower part of widget, which can be fully or partially folded into toolbar at the bottom.
 * @par Example:
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
 * @accessors int count() const
 */
/**
 * @property NavBar::currentIndex
 * This property holds the index of the current page.
 * @accessors int currentIndex() const\n void setCurrentIndex(int)
 * @signals currentChanged(int)
 */
/**
 * @property NavBar::rowHeight
 * This property holds height of a row in a page list.
 * @accessors int rowHeight() const\n void setRowHeight(int)
 */
/**
 * @property NavBar::visibleRows
 * This property holds number of currently visible rows.
 * It's value changed when splitter is moved.
 * @accessors int visibleRows() const\n void setVisibleRows(int)
 * @signals visibleRowsChanged(int)
 */
/**
 * @property NavBar::showHeader
 * This property controls visibility of NavBar header.
 * @accessors bool showHeader() const\n void setShowHeader(bool)
 */
/**
 * @property NavBar::showOptionsMenu
 * This property controls visibility of options menu button in NavBar toolbar.
 * @accessors bool showOptionsMenu() const\n void setShowOptionsMenu(bool)
 */
/**
 * @property NavBar::smallIconSize
 * This property holds size of icons in the bottom toolbar.
 * @accessors int smallIconSize() const\n void setSmallIconSize(int)
 */
/**
 * @property NavBar::largeIconSize
 * This property holds size of icons in the page list.
 * @accessors int smallIconSize() const\n void setSmallIconSize(int)
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
 * @struct NavBar::Page
 * Internal page container
 * @internal
 */

/**
 * Constructs new NavBar
 * @param parent Parent widget
 * @param f WindowFlags, passed to QFrame constructor
 */
NavBar::NavBar(QWidget *parent, Qt::WindowFlags f):
    QFrame(parent, f)
{
    headerVisible  = true;
    optMenuVisible = false;
    headerHeight   = 26;
    pageIconSize   = QSize(24, 24);
    setFrameStyle(QFrame::Panel | QFrame::Sunken);

    header = new NavBarHeader(this);
    header->setFrameStyle(QFrame::Panel | QFrame::Raised);
    stackedWidget = new QStackedWidget(this);
    pageList = new NavBarPageList(this);
    pageToolBar = new NavBarToolBar(this);
    pageToolBar->setMinimumHeight(pageList->rowHeight());

    splitter = new NavBarSplitter(this);
    splitter->setOrientation(Qt::Vertical);
    splitter->addWidget(stackedWidget);
    splitter->addWidget(pageList);
    splitter->setStretchFactor(0, 1);
    splitter->setStretchFactor(1, 0);
    splitter->setCollapsible(0, false);

    resizeContent(size(), rowHeight());

    actionGroup = new QActionGroup(this);
    actionGroup->setExclusive(true);

    connect(actionGroup, SIGNAL(triggered(QAction*)),          SLOT(onClickPageButton(QAction*)));
    connect(pageList,    SIGNAL(buttonVisibilityChanged(int)), SLOT(onButtonVisibilityChanged(int)));

    pagesMenu = new QMenu(this);
}

NavBar::~NavBar()
{
}

int NavBar::count() const
{
    return stackedWidget->count();
}

int NavBar::currentIndex() const
{
    return stackedWidget->currentIndex();
}

/**
 * Returns a pointer to the current widget, or 0 if there is no such page.
 * @return Current widget
 */
QWidget *NavBar::currentWidget() const
{
    return stackedWidget->currentWidget();
}

/**
 * Returns the index of widget, or -1 if the page does not exist.
 * @param widget Widget
 * @return Widget's index
 */
int NavBar::indexOf(QWidget *widget) const
{
    return stackedWidget->indexOf(widget);
}

bool NavBar::showHeader() const
{
    return headerVisible;
}

bool NavBar::showOptionsMenu() const
{
    return optMenuVisible;
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

void NavBar::setShowOptionsMenu(bool show)
{
    if(show != optMenuVisible)
    {
        optMenuVisible = show;
        refillToolBar(visibleRows());
    }
}

int NavBar::rowHeight() const
{
    return pageList->rowHeight();
}

void NavBar::setRowHeight(int height)
{
    if(height < 3)
        height = 3;

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
    if(rows < 0)
        rows = 0;
    if(rows > pages.size())
        rows = pages.size();

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
    return pageIconSize;
}

void NavBar::setLargeIconSize(const QSize &size)
{
    pageIconSize = size;

    for(int i = 0; i < pages.size(); i++)
        pages[i].button->setIconSize(size);
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
 * Returns true if the page at given position is enabled; otherwise returns false.
 * @param index Page index
 * @return Enabled or disabled
 */
bool NavBar::isPageEnabled(int index)
{
    return pages[index].action->isEnabled();
}

/**
 * Creates and inserts new page at given position. If index is -1, page wil be added.
 * @param index Page index
 * @param page Widget
 * @param title Page title
 * @param icon Page icon
 * @return The new page's index
 */
int NavBar::createPage(int index, QWidget *page, const QString &title, const QIcon &icon)
{
    Page p;

    p.action = new QAction(this);
    p.action->setCheckable(true);
    p.action->setText(title);
    p.action->setIcon(icon);

    p.button = new NavBarButton(pageList);
    p.button->setDefaultAction(p.action);
    p.button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    p.button->setToolTip("");
    p.button->setAutoRaise(true);
    p.button->setIconSize(pageIconSize);
    p.button->setGeometry(0, pages.size() * rowHeight(), pageList->width(), rowHeight()); //TODO: move to NavBarPageList
    p.button->setVisible(true);

    int idx;
    if(index < 0) // add page
    {
        p.action->setData(pages.size());
        idx = stackedWidget->addWidget(page);
        pages.append(p);
    }
    else // insert page
    {
        idx = stackedWidget->insertWidget(index, page);
        pages.insert(index, p);

        for(int i = 0; i < pages.size(); i++)
            pages[i].action->setData(i);
    }

    pages[stackedWidget->currentIndex()].action->setChecked(true);
    actionGroup->addAction(p.action);
    header->setText(pages[stackedWidget->currentIndex()].action->text());
    pageList->setMaximumHeight(pages.size() * rowHeight()); //TODO: move to NavBarPageList
    refillToolBar(visibleRows());
    refillPagesMenu();

    return idx;
}

/**
 * Removes widget from the NavBar. i.e., widget is not deleted but simply removed from the navigation bar, causing it to be hidden.
 * @param index Index of widget to be removed
 */
void NavBar::removePage(int index)
{
    if((index < 0) || (index > (pages.size()-1)))
        return;

    int rows = visibleRows();

    stackedWidget->removeWidget(stackedWidget->widget(index));
    actionGroup->removeAction(pages[index].action);
    delete pages[index].button;
    delete pages[index].action;
    pages.removeAt(index);
    pageList->setMaximumHeight(pages.size() * rowHeight()); //TODO: move to NavBarPageList

    if(!pages.isEmpty())
    {
        header->setText(pages[stackedWidget->currentIndex()].action->text());
        pages[stackedWidget->currentIndex()].action->setChecked(true);
    }
    else
        header->setText("");

    if(rows > pages.size())
        setVisibleRows(pages.size());

    refillPagesMenu();
}

/**
 * Returns the title of the page at given position, or an empty string if index is out of range.
 * @param index Page index
 * @return Page title
 */
QString NavBar::pageTitle(int index) const
{
    return pages[index].action->text();
}

/**
 * If enabled is true then the page at given position; otherwise the item at position index is disabled.
 * @param index Page index
 * @param enabled Enable or disable
 */
void NavBar::setPageEnabled(int index, bool enabled)
{
    pages[index].action->setEnabled(enabled);
}

/**
 * Returns the icon of the page at given position, or a null icon if index is out of range.
 * @param index Page index
 * @return Page icon
 */
QIcon NavBar::pageIcon(int index) const
{
    return pages[index].action->icon();
}

/**
 * Sets the title of the page at given position.
 * @param index Page index
 * @param title New title
 */
void NavBar::setPageTitle(int index, const QString &title)
{
    pages[index].action->setText(title);
}

/**
 * Sets the icon of the page at given position.
 * @param index Page index
 * @param icon New icon
 */
void NavBar::setPageIcon(int index, const QIcon &icon)
{
    pages[index].action->setIcon(icon);
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
    if((index < 0) || (index > (pages.size()-1)))
        return;

    stackedWidget->setCurrentIndex(index);
    header->setText(pages[index].action->text());
    pages[index].action->setChecked(true);
    emit currentChanged(index);
}

/**
 * Makes widget the current widget. The widget must be an item in this navbar.
 * @param widget Pointer to widget
 */
void NavBar::setCurrentWidget(QWidget *widget)
{
    if(pages.isEmpty())
        return;

    stackedWidget->setCurrentWidget(widget);
    int index = stackedWidget->currentIndex();
    header->setText(pages[index].action->text());
    pages[index].action->setChecked(true);
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

void NavBar::refillToolBar(int visCount)
{
    pageToolBar->clear();
    QWidget *spacerWidget = new QWidget(this);
    spacerWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    spacerWidget->setVisible(true);
    pageToolBar->addWidget(spacerWidget);

    for(int i = 0; i < pages.size(); i++)
        if(i > visCount-1)
            pageToolBar->addAction(pages[i].action);

    if(optMenuVisible)
    {
        QToolButton *menuBtn = new QToolButton;
        menuBtn->setMaximumWidth(16);
        menuBtn->setAutoRaise(true);
        menuBtn->setPopupMode(QToolButton::InstantPopup);
        menuBtn->setMenu(pagesMenu);
        pageToolBar->addWidget(menuBtn);
    }
}

void NavBar::refillPagesMenu()
{
    pagesMenu->clear();

    for(int i = 0; i < pages.size(); i++)
    {
        QAction *changeVis = new QAction(pagesMenu);
        changeVis->setText(pages[i].action->text());
        changeVis->setCheckable(true);
        changeVis->setChecked(true);
        changeVis->setData(i);
        pagesMenu->addAction(changeVis);
    }
}

void NavBar::onButtonVisibilityChanged(int visCount)
{
    refillToolBar(visCount);
    emit visibleRowsChanged(visCount); //TODO: do not emit this, when rowHeight is changed.
}

/**
 * Helper function, which loads text file and returns it content as QString.
 * There are some style sheets, included in resources under <tt>:/styles/</tt> path:
 *   - office2003gray.css
 *   - office2003blue.css
 *   - office2003green.css
 *   - office2003silver.css
 *   - office2007blue.css
 *   - office2007black.css
 *   - office2007silver.css
 *
 * @par Example:
 * @code
   navBar->setStyleSheet(NavBar::loadStyle(":/styles/office2003gray.css"));
   @endcode
 *
 * @note If you want to use style sheets from navbar resource file, when navbar compiled as static library,
 * you should call <TT>Q_INIT_RESOURCE(navbar)</TT> in your application, see
 * <A HREF="http://qt-project.org/doc/qt-4.8/qdir.html#Q_INIT_RESOURCE">Q_INIT_RESOURCE</A>.
 *
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


/**
 * Constructs new NavBarHeader
 * @param parent Parent widget, passed to QLabel constructor
 * @param f WindowFlags, passed to QLabel constructor
 */
NavBarHeader::NavBarHeader(QWidget *parent, Qt::WindowFlags f):
    QLabel(parent, f)
{
}

/**
 * Constructs new NavBarHeader
 * @param text Header text
 * @param parent Parent widget, passed to QLabel constructor
 * @param f WindowFlags, passed to QLabel constructor
 */
NavBarHeader::NavBarHeader(const QString &text, QWidget *parent, Qt::WindowFlags f):
    QLabel(text, parent, f)
{
}


NavBarToolBar::NavBarToolBar(QWidget *parent):
    QToolBar(parent)
{
}
