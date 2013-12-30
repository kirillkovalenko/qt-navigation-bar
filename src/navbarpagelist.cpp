#include <QResizeEvent>
#include "navbarpagelist.h"

NavBarPageList::NavBarPageList(QWidget *parent) :
    QWidget(parent)
{
    pageButtonHeight = 32;
    QToolButton tmp;
    pageIconSize = tmp.iconSize();
}

NavBarPageList::~NavBarPageList()
{
    for(int i = 0; i < buttons.size(); i++)
    {
        delete buttons[i];
        buttons.removeAt(i);
    }
}

void NavBarPageList::addItem(QAction *action)
{
    buttons.append(createPageButton(action));
    setMaximumHeight(buttons.size() * pageButtonHeight);
}

void NavBarPageList::insertItem(int index, QAction *action)
{
    buttons.insert(index, createPageButton(action));
    setMaximumHeight(buttons.size() * pageButtonHeight);
}

void NavBarPageList::removeItem(int index)
{
    delete buttons[index];
    buttons.removeAt(index);
}

int NavBarPageList::rowHeight() const
{
    return pageButtonHeight;
}

void NavBarPageList::setRowHeight(int newHeight)
{
    pageButtonHeight = newHeight;
    setMaximumHeight(buttons.size() * pageButtonHeight);
}

QSize NavBarPageList::iconSize() const
{
    return pageIconSize;
}

void NavBarPageList::setIconSize(const QSize &size)
{
    pageIconSize = size;

    for(int i = 0; i < buttons.size(); i++)
        buttons[i]->setIconSize(size);
}

void NavBarPageList::resizeEvent(QResizeEvent *e)
{
    int rows = e->size().height() / pageButtonHeight;

    for(int i = 0; i < buttons.size(); i++)
    {
        buttons[i]->setGeometry(0, i * pageButtonHeight, e->size().width(), pageButtonHeight);
        buttons[i]->setToolTip("");
    }

    if(e->oldSize().height() != e->size().height())
        emit buttonVisibilityChanged(rows);

    QWidget::resizeEvent(e);
}

QAbstractButton *NavBarPageList::createPageButton(QAction *action)
{
    NavBarButton *btn = new NavBarButton(this);
    btn->setDefaultAction(action);
    btn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    btn->setToolTip("");
    btn->setAutoRaise(true);
    btn->setIconSize(pageIconSize);
    btn->setGeometry(0, buttons.size() * pageButtonHeight, width(), pageButtonHeight);

    return btn;
}


NavBarButton::NavBarButton(QWidget *parent):
    QToolButton(parent)
{
}
