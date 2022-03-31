#include <QDebug>
#include "panels/editor.h"

Editor::Editor(QObject* parent) : QObject(parent)
{
}

void Editor::canvasScaleChanged(int scale)
{
    canvasScale = scale;
    refreshCanvasImage();
}

void Editor::canvasWidthChanged(int width)
{
    canvasSize.setWidth(width);
    refreshCanvasImage();
}

void Editor::canvasHeightChanged(int height)
{
    canvasSize.setHeight(height);
    refreshCanvasImage();
}

void Editor::refreshCanvasImage()
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

void Editor::mousePressed(Qt::MouseButton button)
{
    qInfo() << button;
}

void Editor::mouseReleased(Qt::MouseButton button)
{
    qInfo() << button;
}
