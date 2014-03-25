#ifndef NAVBAROPTIONSDLG_H
#define NAVBAROPTIONSDLG_H

#include "navbarpage.h"
#include "ui_navbaroptionsdialog.h"

class NavBarOptionsDialog : public QDialog, private Ui::NavBarOptionsDialog
{
    Q_OBJECT

public:
    explicit NavBarOptionsDialog(QWidget *parent = 0);

    void setPageList(QList<NavBarPage> plist);
    QList<NavBarPage> pageList();

    void setDefaultPageOrder(QList<QWidget *> order);

private slots:
    void movePageUp();
    void movePageDown();
    void resetPages();
    void onCurrentRowChanged(int row);

private:
    void fillListWidget();
    QList<NavBarPage> pages;
};

#endif // NAVBAROPTIONSDLG_H
