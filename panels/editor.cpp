#include <QDebug>
#include <regex>
#include "panels/editor.h"
#include "qpainter.h"

Editor::Editor(QWidget* parent) : QWidget(parent)
{
    isDrawing = false;
    currentPenWidth = 1;
    currentColor = Qt::black;
}

//destructor
Editor::~Editor()
{

}
//copy constructor
Editor::Editor(const Editor& other)
{

}
//assignment operator
Editor& Editor:: operator=(const Editor other)
{

}

int Editor:: getIndex()
{
    return index;
}

void Editor::setIndex(int index)
{
    this->index = index;
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
    if (canvasSize.isValid())
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
    // DELETE: Added to allow drawing on canvas
    // Tracks mouse press position on canvas
    if (e->button() == Qt::LeftButton)
    {
        lastPoint = e->pos();
        isDrawing = true;
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

    // DELETE: Added to allow drawing on canvas
    if(e->button() == Qt::LeftButton && isDrawing)
    {
        drawLine(e->pos());
        isDrawing = false;
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
            emit updateViewCanvas(canvas, e->pos() - canvasOffset);
    }

    // DELETE: Added to allow drawing on canvas
    // Tracks mouse movement over canvas
    if ((e->buttons() & Qt::LeftButton) && isDrawing)
    {
        drawLine(e->pos());
        emit updateViewCanvas(canvas, canvasOffset);
    }
}

// Canvas Drawing Implementations

void Editor::drawLine(const QPoint &endPoint)
{
    QPainter painter(&canvas);
    painter.setPen(QPen(currentColor, currentPenWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.drawLine(lastPoint, endPoint);

    int rad = (currentPenWidth / 2) + 2;
    update(QRect(lastPoint, endPoint).normalized().adjusted(-rad, -rad, +rad, +rad));
    lastPoint = endPoint;
}
