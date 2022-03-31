#ifndef VIEW_H
#define VIEW_H

#include <QMainWindow>
#include <QActionGroup>
#include <QPainter>
#include <QImage>
#include "panels/editor.h"

QT_BEGIN_NAMESPACE
namespace Ui { class View; }
QT_END_NAMESPACE

class View : public QMainWindow
{
    Q_OBJECT
public:
    View(Editor& editorPanel, QWidget *parent = nullptr);
    ~View();
private:
    Ui::View* ui;
    QActionGroup* toolActionGroup;
    QImage viewCanvas;
    QPoint viewCanvasOffset;
    // TODO: maybe put this into a .hex file in the res folder and read from there
    std::vector<QString> palette
    {
        "1a1c2c",
        "5d275d",
        "b13e53",
        "ef7d57",
        "ffcd75",
        "a7f070",
        "38b764",
        "257179",
        "29366f",
        "3b5dc9",
        "41a6f6",
        "73eff7",
        "f4f4f4",
        "94b0c2",
        "566c86",
        "333c57"
    };
    void updateViewCanvas(const QImage&, QPoint);
protected:
    void paintEvent(QPaintEvent*);
    void mousePressEvent(QMouseEvent*);
    void mouseReleaseEvent(QMouseEvent*);
    void mouseMoveEvent(QMouseEvent*);
signals:
    void mousePressed(QMouseEvent*);
    void mouseReleased(QMouseEvent*);
    void mouseMoved(QMouseEvent*);
};
#endif // VIEW_H
