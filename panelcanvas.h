#ifndef PANELCANVAS_H
#define PANELCANVAS_H

#include <QObject>
#include <QImage>

class PanelCanvas : public QObject
{
    Q_OBJECT
public:
    explicit PanelCanvas(QObject *parent = nullptr);
public slots:
    void setCanvasScale(int);
    void setCanvasWidth(int);
    void setCanvasHeight(int);
signals:
    void updateCanvasLabel(const QImage&);
private:
    const int canvasCheckerboardSize = 8;
    int canvasScale;
    QSize canvasSize;
    QImage canvas;
    void refreshCanvasImage();
};

#endif // PANELCANVAS_H
