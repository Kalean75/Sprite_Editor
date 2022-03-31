#ifndef PANELCANVAS_H
#define PANELCANVAS_H

#include <QObject>
#include <QImage>

class PanelCanvas : public QObject
{
    Q_OBJECT
public:
    explicit PanelCanvas(QObject *parent = nullptr);
private:
    const int canvasCheckerboardSize = 8;
    int canvasScale;
    QSize canvasSize;
    QImage canvas;
    void refreshCanvasImage();
public slots:
    void canvasScaleChanged(int);
    void canvasWidthChanged(int);
    void canvasHeightChanged(int);
    void mousePressed(Qt::MouseButton);
    void mouseReleased(Qt::MouseButton);
signals:
    void updateViewCanvas(const QImage&);
};

#endif // PANELCANVAS_H
