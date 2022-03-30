#ifndef VIEW_H
#define VIEW_H

#include <QMainWindow>
#include <QActionGroup>
#include "panelcanvas.h"

QT_BEGIN_NAMESPACE
namespace Ui { class View; }
QT_END_NAMESPACE

class View : public QMainWindow
{
    Q_OBJECT

public:
    View(PanelCanvas& canvas, QWidget *parent = nullptr);
    ~View();

private:
    QActionGroup* toolActionGroup;
    Ui::View *ui;
    void updateCanvasLabel(const QImage&);
};
#endif // VIEW_H
