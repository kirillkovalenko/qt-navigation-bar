#include <QResizeEvent>
#include <QDebug>
#include "navbar.h"
#include "navbarpagelist.h"

NavBarPageList::NavBarPageList(NavBar *parent) :
    QWidget(parent)
{
    navBar = parent;
    pageButtonHeight = 32;
}

NavBarPageList::~NavBarPageList()
{
}

int NavBarPageList::rowHeight() const
{
    return pageButtonHeight;
}

void NavBarPageList::setRowHeight(int newHeight)
{
    pageButtonHeight = newHeight;
    setMaximumHeight(navBar->visiblePages().size() * pageButtonHeight);
}

void NavBarPageList::layoutButtons(int width)
{
    for(int i = 0; i < navBar->visiblePages().size(); i++)
    {
        navBar->visiblePages()[i].button->setGeometry(0, i * pageButtonHeight, width, pageButtonHeight);
        navBar->visiblePages()[i].button->setToolTip("");
    }
}

void NavBarPageList::resizeEvent(QResizeEvent *e)
{
    int rows = e->size().height() / pageButtonHeight;

    layoutButtons(e->size().width());

    if(e->oldSize().height() != e->size().height())
        emit buttonVisibilityChanged(rows);

    QWidget::resizeEvent(e);
}

NavBarButton::NavBarButton(QWidget *parent):
    QToolButton(parent)
{
}
