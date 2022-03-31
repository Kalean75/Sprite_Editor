#ifndef EDITOR_H
#define EDITOR_H

#include <QObject>
#include <QImage>
#include <QMouseEvent>

class Editor : public QObject
{
    Q_OBJECT
public:
    explicit Editor(QObject *parent = nullptr);
private:
    const int canvasCheckerboardSize = 8;
    int canvasScale;
    QSize canvasSize;
    QPoint canvasOffset;
    QPoint canvasOffsetBuffer;
    QImage canvas;
    enum tool
    {
        pencil,
        eraser,
        bucket,
        eyedrop,
        select,
        move
    };
    std::map<std::string, tool> toolResolve =
    {
        {"pencil", pencil},
        {"eraser", eraser},
        {"bucket", bucket},
        {"eyedrop", eyedrop},
        {"select", select},
        {"move", move}
    };
    tool activeTool = pencil;
    void refreshCanvas();
public slots:
    void canvasScaleChanged(int);
    void canvasWidthChanged(int);
    void canvasHeightChanged(int);
    void toolSelected();
    void mousePressed(QMouseEvent*);
    void mouseReleased(QMouseEvent*);
    void mouseMoved(QMouseEvent*);
signals:
    void updateViewCanvas(const QImage&, QPoint);
};

#endif // EDITOR_H
