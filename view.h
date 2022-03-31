#ifndef VIEW_H
#define VIEW_H

#include <QMainWindow>
#include <QMouseEvent>
#include <QActionGroup>
#include <QPainter>
#include <QImage>
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
    QImage viewCanvas;
    void updateViewCanvas(const QImage&);
protected:
    void paintEvent(QPaintEvent*);
    void mousePressEvent(QMouseEvent*);
    void mouseReleaseEvent(QMouseEvent*);
    void mouseMoveEvent(QMouseEvent*);
signals:
    void mousePressed(Qt::MouseButton);
    void mouseReleased(Qt::MouseButton);
    void mouseMoved(QMouseEvent*);
};
#endif // VIEW_H
