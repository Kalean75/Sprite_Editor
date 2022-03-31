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
    void setCanvasScale(int);
    void setCanvasWidth(int);
    void setCanvasHeight(int);
    void mousePressed(Qt::MouseButton button);
    void mouseReleased(Qt::MouseButton button);
signals:
    void updateCanvasLabel(const QImage&);
};

#endif // PANELCANVAS_H
