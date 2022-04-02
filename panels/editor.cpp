#include <QDebug>
#include <regex>
#include "panels/editor.h"

Editor::Editor(QWidget* parent) : QWidget(parent)
{
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
    // TODO: proper logic for resizing (preserve data when expanded, clip according to new dimensions)
    pixelBuffer.fill(emptyPixel, canvasSize.width() * canvasSize.height());
    refreshCanvas();
}

void Editor::canvasAnchorChanged(QPoint area)
{
    // Canvas refresh not needed because anchor does not affect the pixel buffer
    canvasAnchor = area;
}

void Editor::toolSelected()
{
    // Toolbar actions are of the form "action___" e.g. actionPencil. We can't pass this through the signal, so we
    // retrieve the object name from the signalling action and match it
    std::string actionName = qobject_cast<QObject*>(sender())->objectName().toStdString();
    std::string actionID = std::regex_replace(actionName, std::regex("action([A-Za-z]+)$"), "$1");
    actionID[0] = tolower(actionID[0]);
    activeTool = toolResolve[actionID];
    switch (activeTool)
    {
    case pencil:
        // TODO: load toolColor from palette panel
        toolColor = Qt::red;
        break;
    case eraser:
        // Implicit QColor cast loses alpha channel, so explicit fromRgba call is needed
        toolColor = QColor::fromRgba(emptyPixel);
        break;
    case bucket:
        break;
    case eyedrop:
        break;
    case select:
        break;
    case move:
        break;
    }
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
                int pixel = (y / canvasScale) * canvasSize.width() + x / canvasScale;
                if (pixel < (int) pixelBuffer.size() && pixelBuffer.at(pixel) != emptyPixel)
                {
                    color = pixelBuffer.at(pixel);
                }
                canvas.setPixel(x, y, color.rgba());
            }
        }
        emit updateViewCanvas(canvas, canvasOffset);
    }
}

int Editor::toolPointToPixelIndex()
{
    return toolPoint.y() * canvasSize.width() + toolPoint.x();
}

void Editor::mousePressed(QMouseEvent* e)
{
    switch (activeTool)
    {
    case pencil:
    case eraser:
        pixelBuffer[toolPointToPixelIndex()] = toolColor.rgba();
        refreshCanvas();
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
    QPoint canvasOrigin = canvasAnchor + canvasOffset - canvas.rect().center();
    if (QRect(canvasOrigin, canvas.size()).contains(e->pos(), true))
    {
        toolPoint = (e->pos() - canvasOrigin);
        // Default QPoint divide overload rounds integers up, so we have to do manual integer division
        toolPoint.rx() /= canvasScale;
        toolPoint.ry() /= canvasScale;
    }
    if (e->buttons() & Qt::LeftButton)
    {
        switch (activeTool)
        {
        case pencil:
        case eraser:
            pixelBuffer[toolPointToPixelIndex()] = toolColor.rgba();
            refreshCanvas();
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
    }
}
