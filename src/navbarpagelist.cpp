#include <QResizeEvent>
#include "navbarpagelist.h"

NavBarPageList::NavBarPageList(QWidget *parent) :
    QWidget(parent)
{
    pageButtonHeight = 32;
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
}
/*
QSize NavBarPageList::iconSize() const
{
    if(buttons.empty())
        return QSize(16, 16);
    else
        return buttons[0]->iconSize();
}

void NavBarPageList::setIconSize(const QSize &size)
{
    for(int i = 0; i < buttons.size(); i++)
        buttons[i]->setIconSize(size);
}
*/
void NavBarPageList::resizeEvent(QResizeEvent *e)
{
    int rows = e->size().height() / pageButtonHeight;

    for(int i = 0; i < buttons.size(); i++)
    {
        buttons[i]->setGeometry(0, i * pageButtonHeight, e->size().width(), pageButtonHeight);
        buttons[i]->setVisible(i < rows);
        buttons[i]->setToolTip("");
    }

    if(e->oldSize().height() != e->size().height())
        emit buttonVisibilityChanged(rows);

    QWidget::resizeEvent(e);
}

void NavBarPageList::changeButtonsVisibility(int visRows)
{
    for(int i = 0; i < buttons.size(); i++)
        buttons[i]->setVisible(i < visRows);
}

QAbstractButton *NavBarPageList::createPageButton(QAction *action)
{
    QToolButton *btn = new QToolButton(this);
    btn->setDefaultAction(action);
    btn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    btn->setToolTip("");
    btn->setAutoRaise(true);
    btn->setGeometry(0, buttons.size() * pageButtonHeight, width(), pageButtonHeight);

    return btn;
}
