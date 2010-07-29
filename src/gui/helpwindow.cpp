#include "helpwindow.h"
#include "ui_helpwindow.h"

HelpWindow::HelpWindow(QWidget *parent)
    : QWidget(parent), ui(new Ui::HelpWindowClass)
{
    ui->setupUi(this);
}

HelpWindow::~HelpWindow()
{
    delete ui;
}

void HelpWindow::setHelpFile(QString fileName)
{
	ui->webView->setUrl(fileName);
}
