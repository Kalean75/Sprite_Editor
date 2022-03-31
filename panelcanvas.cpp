#include <QDebug>
#include "panelcanvas.h"

PanelCanvas::PanelCanvas(QObject* parent) : QObject(parent)
{
}

void PanelCanvas::canvasScaleChanged(int scale)
{
    canvasScale = scale;
    refreshCanvasImage();
}

void PanelCanvas::canvasWidthChanged(int width)
{
    canvasSize.setWidth(width);
    refreshCanvasImage();
}

void PanelCanvas::canvasHeightChanged(int height)
{
    canvasSize.setHeight(height);
    refreshCanvasImage();
}

void PanelCanvas::refreshCanvasImage()
{
    canvas = QImage(canvasSize, QImage::Format_ARGB32_Premultiplied).scaled(canvasSize * canvasScale);
    for (int x = 0; x < canvas.width(); x++)
    {
        for (int y = 0; y < canvas.height(); y++)
        {
            QColor color = (x / canvasCheckerboardSize + y / canvasCheckerboardSize) % 2 == 0 ? Qt::lightGray : Qt::white;
            canvas.setPixel(x, y, color.rgb());
        }
    }
    emit updateViewCanvas(canvas);
}

void PanelCanvas::mousePressed(Qt::MouseButton button)
{
    qInfo() << button;
}

void PanelCanvas::mouseReleased(Qt::MouseButton button)
{
    qInfo() << button;
}
