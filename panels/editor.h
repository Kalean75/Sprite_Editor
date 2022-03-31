#ifndef EDITOR_H
#define EDITOR_H

#include <QObject>
#include <QImage>

class Editor : public QObject
{
    Q_OBJECT
public:
    explicit Editor(QObject *parent = nullptr);
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

#endif // EDITOR_H
