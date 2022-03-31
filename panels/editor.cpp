#include <QDebug>
#include <regex>
#include "panels/editor.h"

Editor::Editor(QObject* parent) : QObject(parent)
{
}

void Editor::canvasScaleChanged(int scale)
{
    canvasScale = scale;
    refreshCanvas();
}

void Editor::canvasWidthChanged(int width)
{
    canvasSize.setWidth(width);
    refreshCanvas();
}

void Editor::canvasHeightChanged(int height)
{
    canvasSize.setHeight(height);
    refreshCanvas();
}

void Editor::toolSelected()
{
    // Toolbar actions are of the form "action___" e.g. actionPencil. We can't pass this through the signal, so we
    // retrieve the object name from the signalling action and match it
    std::string actionName = qobject_cast<QObject*>(sender())->objectName().toStdString();
    std::string actionID = std::regex_replace(actionName, std::regex("action([A-Za-z]+)$"), "$1");
    actionID[0] = tolower(actionID[0]);
    activeTool = toolResolve[actionID];
}

void Editor::refreshCanvas()
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
    emit updateViewCanvas(canvas, canvasOffset);
}

void Editor::mousePressed(QMouseEvent* e)
{
    switch (activeTool)
    {
        case pencil:
            break;
        case eraser:
            break;
        case bucket:
            break;
        case eyedrop:
            break;
        case select:
            break;
        case move:
            canvasOffset = e->pos() - canvasOffset;
    }
}

void Editor::mouseReleased(QMouseEvent* e)
{
    switch (activeTool)
    {
        case pencil:
            break;
        case eraser:
            break;
        case bucket:
            break;
        case eyedrop:
            break;
        case select:
            break;
        case move:
            canvasOffset = e->pos() - canvasOffset;
            emit updateViewCanvas(canvas, canvasOffset);
    }
}

void Editor::mouseMoved(QMouseEvent* e)
{
    switch (activeTool)
    {
        case pencil:
            break;
        case eraser:
            break;
        case bucket:
            break;
        case eyedrop:
            break;
        case select:
            break;
        case move:
            canvasOffsetBuffer = e->pos();
            emit updateViewCanvas(canvas, canvasOffsetBuffer - canvasOffset);
    }
}
