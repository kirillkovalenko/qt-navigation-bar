#include <QStylePainter>
#include <QStyleOptionButton>
#include <QFontMetrics>
#include "navbarheader.h"


/**
 * @class NavBarHeader
 * @brief Navigation bar header.
 *
 * Navigation bar header
 */

/**
 * Constructs new NavBarHeader
 * @param parent Parent widget, passed to QLabel constructor
 * @param f WindowFlags, passed to QLabel constructor
 */
NavBarHeader::NavBarHeader(QWidget *parent, Qt::WindowFlags f):
    QLabel(parent, f)
{
    createButton();
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
    createButton();
}

void NavBarHeader::resizeEvent(QResizeEvent *e)
{
    button->setGeometry(width()-24, 2, 22, 22);
    QLabel::resizeEvent(e);
}

void NavBarHeader::createButton()
{
    button = new QToolButton(this);
    button->setCheckable(true);
    connect(button, SIGNAL(clicked(bool)), SIGNAL(buttonClicked(bool)));
}


NavBarTitleButton::NavBarTitleButton(QWidget *parent):
    QPushButton(parent)
{

}

QSize NavBarTitleButton::sizeHint() const
{
    return QSize(-1, -1);
}

QSize NavBarTitleButton::minimumSizeHint() const
{
    return QSize(-1, -1);
}

void NavBarTitleButton::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e);

    QStylePainter p(this);
    QStyleOptionButton opt;
    initStyleOption(&opt);

    p.drawControl(QStyle::CE_PushButtonBevel, opt);

    p.setFont(font());
    QFontMetrics fm(font());
    QRect r = fm.boundingRect(text());
    p.translate(width()/2 + r.height()/2, height()/2 + r.width()/2);
    p.rotate(270);
    p.drawText(0, 0, text());
}
