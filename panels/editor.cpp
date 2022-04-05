#include <QDebug>
#include <regex>
#include "panels/editor.h"

Editor::Editor(QWidget* parent) : QWidget(parent)
{
    //set initial width and height
    canvasSize.setHeight(32);
    canvasSize.setWidth(32);
    //make sure all pixels are "transparent"
    pixelBuffer.clear();
    pixelBuffer.fill(transparentPixel, canvasSize.width() * canvasSize.height());
    QPoint canvasOrigin = canvasAnchor + canvasOffset - canvas.rect().center();
}

//destructor
Editor::~Editor()
{
    //TODO
    //put stuff here
}
//copy constructor
Editor::Editor(const Editor& other)
{
    canvas = other.canvas;
    index = other.index;
    canvasSize = other.canvasSize;
    canvasOffset = other.canvasOffset;
    canvasAnchor = other.canvasAnchor;
    pixelBuffer = other.pixelBuffer;
}
//assignment operator
Editor& Editor:: operator=( Editor other)
{
    std::swap(canvas, other.canvas);
    std::swap(index, other.index);
    std::swap(canvasSize, other.canvasSize);
    std::swap(canvasOffset, other.canvasOffset);
    std::swap(canvasAnchor, other.canvasAnchor);
   std::swap(pixelBuffer, other.pixelBuffer);
    return *this;
}
//gets the index of the current frame within the vector
//returns index
int Editor:: getIndex()
{
    return index;
}
//sets the index of the current frame in the vector
void Editor::setIndex(int index)
{
    this->index = index;
}

void Editor::canvasScaleChanged(int scale)
{
    canvasScale = scale;
    refreshCanvas();
    emit serializeValue(Serialization::ZoomScale, scale);
}

void Editor::canvasWidthChanged(int width)
{
    canvasSize.setWidth(width);
    //pixelBuffer.fill(transparentPixel, canvasSize.width() * canvasSize.height());
    refreshCanvas();
    emit serializeValue(Serialization::Width, width);
}

void Editor::canvasHeightChanged(int height)
{
    canvasSize.setHeight(height);
    // TODO: proper logic for resizing (preserve data when expanded, clip according to new dimensions)
    //pixelBuffer.fill(transparentPixel, canvasSize.width() * canvasSize.height());
    refreshCanvas();
    emit serializeValue(Serialization::Height, height);
}

void Editor::canvasAnchorChanged(QPoint area)
{
    // Canvas refresh not needed because anchor does not affect the pixel buffer
    canvasAnchor = area;
}

void Editor::canvasOffsetChanged(QPoint offset)
{
    canvasOffset = offset;
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
                int pixelIndex = (y / canvasScale) * canvasSize.width() + x / canvasScale;
                if (pixelIndex < (int) pixelBuffer.size() && pixelBuffer.at(pixelIndex) != transparentPixel)
                {
                    color = pixelBuffer.at(pixelIndex);
                }
                canvas.setPixel(x, y, color.rgba());
            }
        }
        emit updateViewCanvas(canvas, canvasOffset);
        emit serializeValue(Serialization::Frames, getSerializedFrame());
    }
}

int Editor::currentPixelIndex()
{
    return toolPoint.y() * canvasSize.width() + toolPoint.x();
}

void Editor::bucketFill(QRgb color, QRgb newColor, int pixelIndex)
{
    int rowShift = canvasSize.width();
    if (pixelIndex < 0 || pixelIndex >= (int) pixelBuffer.size())
    {
        return;
    }
    else if (pixelBuffer[pixelIndex] != color)
    {
        return;
    }
    else if (pixelBuffer[pixelIndex] == newColor)
    {
        return;
    }
    pixelBuffer[pixelIndex] = newColor;
    bucketFill(color, newColor, pixelIndex + rowShift);
    bucketFill(color, newColor, pixelIndex - rowShift);
    bucketFill(color, newColor, pixelIndex + 1);
    bucketFill(color, newColor, pixelIndex - 1);
}

QJsonArray Editor::getSerializedFrame()
{
    QJsonArray frameData = {index};
    for (int y = 0; y < canvasSize.height(); y++)
    {
        QJsonArray row;
        for (int x = 0; x < canvasSize.width(); x++)
        {
            QColor pixel = QColor::fromRgba(pixelBuffer.at(canvasSize.width() * y + x));
            QJsonArray pixelData = {pixel.red(), pixel.green(), pixel.blue(), pixel.alpha()};
            row.append(pixelData);
        }
        frameData.append(row);
    }
    return frameData;
}

void Editor::mousePressed(QMouseEvent* e)
{
    int pixelIndex = currentPixelIndex();
    QRgb& pixelColor = pixelBuffer[pixelIndex];
    switch (activeTool)
    {
    case pencil:
        pixelColor = toolColor.rgba();
        refreshCanvas();
        break;
    case eraser:
        pixelColor = transparentPixel;
        refreshCanvas();
        break;
    case bucket:
        // TODO: when color selection logic is ready, replace "QColor(Qt::darkGray)" with "toolColor"
        bucketFill(pixelColor, toolColor.rgba(), pixelIndex);
        refreshCanvas();
        break;
    case eyedrop:
        if (pixelColor != transparentPixel)
        {
            toolColor = pixelColor;
        }
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
//        emit updatePreview();
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
        QRgb& pixelColor = pixelBuffer[currentPixelIndex()];
        switch (activeTool)
        {
        case pencil:
            pixelColor = toolColor.rgba();
            refreshCanvas();
            break;
        case eraser:
            pixelColor = transparentPixel;
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

void Editor::colorSelected(QColor color)
{
    toolColor = color;
}
//returns the image on the canvas
QImage Editor::getImage()
{
    return canvas;
}
int Editor::getWidth()
{
    return canvasSize.width();
}

int Editor::getHeight()
{
    return canvasSize.height();
}
