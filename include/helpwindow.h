#ifndef HELPWINDOW_H
#define HELPWINDOW_H

#include <QtGui/QWidget>

namespace Ui
{
    class HelpWindowClass;
}

class HelpWindow : public QWidget
{
    Q_OBJECT

public:
    HelpWindow(QWidget *parent = 0);
    ~HelpWindow();
	
	void setHelpFile(QString fileName);

private:
    Ui::HelpWindowClass *ui;
};

#endif // HELPWINDOW_H
