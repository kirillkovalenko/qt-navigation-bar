#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include "wnd.h"

Wnd::Wnd(QWidget *parent)
    : QWidget(parent)
{
    navBar = new NavBar;
    navBar->setStyleSheet(NavBar::loadStyle(":/styles/office2003gray.css"));

    navBar->addPage(new QLabel("This is page 1"), "Page 1", QIcon(":/images/mail_yellow.png"));
    navBar->addPage(new QLabel("This is page 2"), "Page 2", QIcon(":/images/calendar.png"));
    navBar->addPage(new QLabel("This is page 3"), "Page 3", QIcon(":/images/client_account_template.png"));
    navBar->addPage(new QLabel("This is page 4"), "Page 4", QIcon(":/images/date_task.png"));
    navBar->addPage(new QLabel("This is page 5"), "Page 5", QIcon(":/images/note.png"));

    navBar->setVisibleRows(3);

    connect(navBar, SIGNAL(currentChanged(int)),     SLOT(navBarCurrentChanged(int)));
    connect(navBar, SIGNAL(visibleRowsChanged(int)), SLOT(navBarVisibleRowsChanged(int)));

    showHeaderBox = new QCheckBox;
    showHeaderBox->setText("Show header");
    showHeaderBox->setChecked(true);
    connect(showHeaderBox, SIGNAL(toggled(bool)), navBar, SLOT(setShowHeader(bool)));

    showOptMenuBox = new QCheckBox;
    showOptMenuBox->setText("Show options menu button");
    showOptMenuBox->setChecked(false);
    connect(showOptMenuBox, SIGNAL(toggled(bool)), navBar, SLOT(setShowOptionsMenu(bool)));

    styleBox = new QComboBox;
    styleBox->addItem("Office 2003 Gray",   ":/styles/office2003gray.css");
    styleBox->addItem("Office 2003 Blue",   ":/styles/office2003blue.css");
    styleBox->addItem("Office 2003 Silver", ":/styles/office2003silver.css");
    styleBox->addItem("Office 2003 Green",  ":/styles/office2003green.css");
    styleBox->addItem("Office 2007 Blue",   ":/styles/office2007blue.css");
    styleBox->addItem("Office 2007 Black",  ":/styles/office2007black.css");
    styleBox->addItem("Office 2007 Silver", ":/styles/office2007silver.css");
    styleBox->addItem("None",               "");
    connect(styleBox, SIGNAL(currentIndexChanged(int)), SLOT(changeStylesheet(int)));

    signalWidget = new QListWidget;

    QVBoxLayout *vLayout = new QVBoxLayout;
    QHBoxLayout *hLayout = new QHBoxLayout;
    vLayout->addWidget(styleBox);
    vLayout->addWidget(showHeaderBox);
    vLayout->addWidget(showOptMenuBox);
    vLayout->addWidget(signalWidget);
    hLayout->addWidget(navBar);
    hLayout->addLayout(vLayout);
    setLayout(hLayout);
}

Wnd::~Wnd()
{    
}

void Wnd::changeStylesheet(int index)
{
    navBar->setStyleSheet(NavBar::loadStyle(styleBox->itemData(index).toString()));
}

void Wnd::navBarCurrentChanged(int index)
{
    signalWidget->addItem(QString("currentChanged(%1)").arg(index));
    signalWidget->scrollToBottom();
}

void Wnd::navBarVisibleRowsChanged(int rows)
{
    signalWidget->addItem(QString("visibleRowsChanged(%1)").arg(rows));
    signalWidget->scrollToBottom();
}
