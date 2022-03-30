#ifndef VIEW_H
#define VIEW_H

#include <QMainWindow>
#include <QActionGroup>

QT_BEGIN_NAMESPACE
namespace Ui { class View; }
QT_END_NAMESPACE

class View : public QMainWindow
{
    Q_OBJECT

public:
    View(QWidget *parent = nullptr);
    ~View();

private:
    Ui::View *ui;
    QActionGroup* toolActionGroup;
};
#endif // VIEW_H
