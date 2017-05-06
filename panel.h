#ifndef PANEL_H
#define PANEL_H

#include <QMainWindow>

namespace Ui {
class Panel;
}

class Panel : public QMainWindow
{
    Q_OBJECT

public:
    explicit Panel(QWidget *parent = 0);
    ~Panel();

private:
    Ui::Panel *ui;
};

#endif // PANEL_H
