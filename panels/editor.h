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
    enum tool
    {
        pencil,
        eraser,
        bucket,
        eyedrop,
        select
    };
    std::map<std::string, tool> toolResolve =
    {
        {"pencil", pencil},
        {"eraser", eraser},
        {"bucket", bucket},
        {"eyedrop", eyedrop},
        {"select", select}
    };
    tool activeTool = pencil;
    void refreshCanvas();
public slots:
    void canvasScaleChanged(int);
    void canvasWidthChanged(int);
    void canvasHeightChanged(int);
    void toolSelected();
    void mousePressed(Qt::MouseButton);
    void mouseReleased(Qt::MouseButton);
signals:
    void updateViewCanvas(const QImage&);
};

#endif // EDITOR_H
