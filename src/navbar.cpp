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
 * Internal page structure
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

/**
 * @property NavBar::count
 * Number of pages in navigation bar.
 * @access int count() const
 */
int NavBar::count() const
{
    return stackedWidget->count();
}

/**
 * @property NavBar::currentIndex
 * This property holds the index of the current page.
 * @access int currentIndex() const\n void setCurrentIndex(int)
 * @signals currentChanged(int)
 */
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

/**
 * @property NavBar::showHeader
 * This property controls visibility of NavBar header.
 * @access bool showHeader() const\n void setShowHeader(bool)
 */
bool NavBar::showHeader() const
{
    return headerVisible;
}

/**
 * @property NavBar::showOptionsMenu
 * This property controls visibility of options menu button in NavBar toolbar.
 * @access bool showOptionsMenu() const\n void setShowOptionsMenu(bool)
 */
bool NavBar::showOptionsMenu() const
{
    return optMenuVisible;
}

/**
 * Toggles visibility of navbar header
 * @param show Show or hide
 */
void NavBar::setShowHeader(bool show)
{
    if(show != headerVisible)
    {
        headerVisible = show;
        header->setVisible(show);
        resizeContent(size(), rowHeight());
    }
}

/**
 * Toggles visibility of options menu button in NavBar toolbar.
 * @param show Show or hide
 */
void NavBar::setShowOptionsMenu(bool show)
{
    if(show != optMenuVisible)
    {
        optMenuVisible = show;
        refillToolBar(visibleRows());
    }
}

/**
 * @property NavBar::rowHeight
 * This property holds height of a row in a page list.
 * @access int rowHeight() const\n void setRowHeight(int)
 */
int NavBar::rowHeight() const
{
    return pageList->rowHeight();
}

/**
 * Set height of a row in a page list.
 * @param height Row height
 */
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

/**
 * @property NavBar::visibleRows
 * This property holds number of currently visible rows.
 * It's value changed when splitter is moved.
 * @access int visibleRows() const\n void setVisibleRows(int)
 * @signals visibleRowsChanged(int)
 */
int NavBar::visibleRows() const
{
    return pageList->height() / rowHeight();
}

/**
 * Set number of currently visible rows.
 * @param rows Number of rows
 */
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

void NavBar::recalcPageList()
{
    pageList->setMaximumHeight(visiblePages().size() * rowHeight());
    pageList->layoutButtons(pageList->width());

    for(int i = 0; i < pages.size(); i++)
        pages[i].action->setData(i);
}

/**
 * @property NavBar::smallIconSize
 * This property holds size of icons in the bottom toolbar.
 * @access int smallIconSize() const\n void setSmallIconSize(int)
 */
QSize NavBar::smallIconSize() const
{
    return pageToolBar->iconSize();
}

/**
 * Sets size of icons in the page list.
 * @param size Icon size
 */
void NavBar::setSmallIconSize(const QSize &size)
{
    pageToolBar->setIconSize(size);
    pageToolBar->setMinimumHeight(pageList->rowHeight());
}

/**
 * @property NavBar::largeIconSize
 * This property holds size of icons in the page list.
 * @access int smallIconSize() const\n void setSmallIconSize(int)
 */
QSize NavBar::largeIconSize() const
{
    return pageIconSize;
}

/**
 * Sets size of icons in the bottom toolbar.
 * @param size Icon size
 */
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
 * @param text Page text
 * @return The new page's index
 */
int NavBar::addPage(QWidget *page, const QString &text)
{
    return addPage(page, text, QIcon());
}

/**
 * Adds widget as new page to navigation bar
 * @param page Widget to be added as new page
 * @param text Page text
 * @param icon Page icon
 * @return The new page's index
 */
int NavBar::addPage(QWidget *page, const QString &text, const QIcon &icon)
{
    return createPage(-1, page, text, icon);
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
 * @param text Page text
 * @return The new page's index
 */
int NavBar::insertPage(int index, QWidget *page, const QString &text)
{
    return insertPage(index, page, text, QIcon());
}

/**
 * Inserts new page at given position, or at the bottom of the navigation bar if index is out of range.
 * @param index Page position
 * @param page Widget
 * @param text Page text
 * @param icon Page icon
 * @return The new page's index
 */
int NavBar::insertPage(int index, QWidget *page, const QString &text, const QIcon &icon)
{
    return createPage(index, page, text, icon);
}

/**
 * Creates and inserts new page at given position. If index is -1, page wil be added.
 * @param index Page index
 * @param page Widget
 * @param text Page text
 * @param icon Page icon
 * @return The new page's index
 */
int NavBar::createPage(int index, QWidget *page, const QString &text, const QIcon &icon)
{
    Page p;

    p.action = new QAction(this);
    p.action->setCheckable(true);
    p.action->setText(text);
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
    }

    pages[stackedWidget->currentIndex()].action->setChecked(true);
    actionGroup->addAction(p.action);
    header->setText(pages[stackedWidget->currentIndex()].text());
    recalcPageList();
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
    recalcPageList();

    if(!pages.isEmpty())
    {
        header->setText(pages[stackedWidget->currentIndex()].text());
        pages[stackedWidget->currentIndex()].action->setChecked(true);
    }
    else
        header->setText("");

    if(rows > pages.size())
        setVisibleRows(pages.size());

    refillToolBar(visibleRows());
    refillPagesMenu();
}

/**
 * Returns the text of the page at given position, or an empty string if index is out of range.
 * @param index Page index
 * @return Page text
 */
QString NavBar::pageText(int index) const
{
    return pages[index].text();
}

/**
 * Returns true if the page at given position is enabled; otherwise returns false.
 * @param index Page index
 * @return Enabled or disabled
 */
bool NavBar::isPageEnabled(int index)
{
    return pages[index].isEnabled();
}

/**
 * If visible is true then the page at given position is visible; otherwise the page at position index is invisible.
 * @param index Page index
 * @param enabled Enable or disable
 */
void NavBar::setPageVisible(int index, bool visible)
{
    pages[index].setVisible(visible);
    recalcPageList();
    refillToolBar(visibleRows());
}

/**
 * Returns true if the page at given position is visible; otherwise returns false.
 * @param index Page index
 * @return Visible or not
 */
bool NavBar::isPageVisible(int index)
{
    return pages[index].isVisible();
}

/**
 * If enabled is true then the page at given position is enabled; otherwise the page at position index is disabled.
 * @param index Page index
 * @param enabled Enable or disable
 */
void NavBar::setPageEnabled(int index, bool enabled)
{
    pages[index].setEnabled(enabled);
}

/**
 * Returns the icon of the page at given position, or a null icon if index is out of range.
 * @param index Page index
 * @return Page icon
 */
QIcon NavBar::pageIcon(int index) const
{
    return pages[index].icon();
}

/**
 * Sets the text of the page at given position.
 * @param index Page index
 * @param text New text
 */
void NavBar::setPageText(int index, const QString &text)
{
    pages[index].setText(text);
}

/**
 * Sets the icon of the page at given position.
 * @param index Page index
 * @param icon New icon
 */
void NavBar::setPageIcon(int index, const QIcon &icon)
{
    pages[index].setIcon(icon);
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

/**
 * Changes active page.
 * @param index Page index
 */
void NavBar::setCurrentIndex(int index)
{
    if((index < 0) || (index > (pages.size()-1)))
        return;

    stackedWidget->setCurrentIndex(index);
    header->setText(pages[index].text());
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
    header->setText(pages[index].text());
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

    for(int i = 0; i < visiblePages().size(); i++)
        if(i > visCount-1)
            pageToolBar->addAction(visiblePages()[i].action);

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
        changeVis->setText(pages[i].text());
        changeVis->setCheckable(true);
        changeVis->setChecked(pages[i].isVisible());
        changeVis->setData(i);
        pagesMenu->addAction(changeVis);
        connect(pagesMenu, SIGNAL(triggered(QAction*)), SLOT(changePageVisibility(QAction*)));
    }
}

QList<NavBar::Page> NavBar::visiblePages()
{
    QList<Page> l;

    for(int i = 0; i < pages.size(); i++)
        if(pages[i].isVisible())
            l.append(pages[i]);

    return l;
}

void NavBar::onButtonVisibilityChanged(int visCount)
{
    refillToolBar(visCount);
    emit visibleRowsChanged(visCount); //TODO: do not emit this, when rowHeight is changed.
}

void NavBar::changePageVisibility(QAction *action)
{
    setPageVisible(action->data().toInt(), action->isChecked());
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
