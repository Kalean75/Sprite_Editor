#ifndef EDITOR_H
#define EDITOR_H

#include <QWidget>
#include <QObject>
#include <QImage>
#include <QMouseEvent>
#include "Serialization.h"

class Editor : public QWidget
{
    Q_OBJECT
public:
    explicit Editor(QWidget *parent = nullptr);
    ~Editor();
    Editor(const Editor& other);
    Editor& operator=(const Editor other);
    int getIndex();
    void setIndex(int index);
    //change to private
    void refreshCanvas();
    QImage getImage();
    int getHeight();
    int getWidth();
private:
    int index;
    const QRgb transparentPixel = qRgba(0, 0, 0, 0);
    const int canvasCheckerboardSize = 8;
    int canvasScale;
    QSize canvasSize;
    QPoint canvasOffset;
    QPoint canvasAnchor;
    QImage canvas;
    QVector<QRgb> pixelBuffer;
    enum Tool
    {
        Pencil,
        Eraser,
        Bucket,
        Eyedrop,
        Select,
        Move
    };
    std::map<std::string, Tool> toolResolve =
    {
        {"pencil", Pencil},
        {"eraser", Eraser},
        {"bucket", Bucket},
        {"eyedrop", Eyedrop},
        {"select", Select},
        {"move", Move}
    };
    Tool activeTool = Pencil;
    QPoint toolPoint;
    // TODO: load toolColor from palette panel
    QColor toolColor = Qt::black;
    //void refreshCanvas();
    int currentPixelIndex();
    void bucketFill(QRgb, QRgb, int);

//    // Current pen settings
//    int currentPenWidth;
//    QColor currentColor;
//    QPoint lastPoint;

//    // Drawing controls
//    bool isDrawing;
//    void drawLine(const QPoint &endPoint);

public slots:
    void canvasScaleChanged(int);
    void canvasWidthChanged(int);
    void canvasHeightChanged(int);
    void canvasAnchorChanged(QPoint);
    void canvasOffsetChanged(QPoint);
    void toolSelected();
    void mousePressed(QMouseEvent*);
    void mouseReleased(QMouseEvent*);
    void mouseMoved(QMouseEvent*);
    void colorSelected(QColor);

signals:
    void updateViewCanvas(const QImage&, QPoint);
    void updatePreview();
    void serializeValue(Serialization::Key, QJsonValue);
};

#endif // EDITOR_H
