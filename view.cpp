#include "view.h"
#include "ui_view.h"

View::View(Editor& editorPanel, Palette& palettePanel, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::View)
{
    ui->setupUi(this);
    toolActionGroup = new QActionGroup(ui->toolbar);
    foreach(QAction* action, ui->toolbar->actions())
    {
        toolActionGroup->addAction(action);
        connect(action, &QAction::triggered, &editorPanel, &Editor::toolSelected);
    }
    connect(ui->zoomSlider, &QSlider::valueChanged, &editorPanel, &Editor::canvasScaleChanged);
    connect(&editorPanel, &Editor::updateViewCanvas, this, &View::updateViewCanvas);
    connect(&palettePanel, &Palette::updateViewPalette, this, &View::updateViewPalette);
    connect(this, &View::mousePressed, &editorPanel, &Editor::mousePressed);
    connect(this, &View::mouseReleased, &editorPanel, &Editor::mouseReleased);
    connect(this, &View::mouseMoved, &editorPanel, &Editor::mouseMoved);
    connect(this,&View::pressedAddFrame,&frame,&Frame::addNewFrame);
    // Establish default values for various components
    // TODO: connect canvas methods to width and height sliders, move default values to serializer class
    ui->toolbar->setStyleSheet("QToolButton { margin: 5px; padding: 2px; }");
    ui->zoomSlider->setValue(8); // canvas.setCanvasScale(8)
    editorPanel.canvasWidthChanged(64);
    editorPanel.canvasHeightChanged(64);
    palettePanel.paletteColumnsChanged(5);
    palettePanel.paletteRowsChanged(5);

    ui->frameslist->addItem("Frame " + QString:: number(frame.currentFrame.getIndex()));
}

View::~View()
{
    delete ui;
}

void View::updateViewCanvas(const QImage& canvas, QPoint canvasOffset)
{
    viewCanvas = canvas;
    viewCanvasOffset = canvasOffset;
    repaint();
}

void View::updateViewPalette(const QVector<QString>& palette, QSize paletteSize)
{
    QTableWidget* table = ui->paletteTable;
    int columns = paletteSize.width();
    int rows = paletteSize.height();
    table->setColumnCount(columns);
    table->setRowCount(rows);
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < columns; j++)
        {
            QTableWidgetItem* item = new QTableWidgetItem;
            int index = columns * i + j;
            item->setBackground(index >= (int) palette.size() ? Qt::white : QColor("#" + palette.at(index)));
            item->setFlags(item->flags() ^ Qt::ItemIsEditable);
            // This can behave destructively. Maybe enforce constraint so that table area always >= initial area
            table->removeCellWidget(i, j);
            table->setItem(i, j, item);
        }
    }
}

void View::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    QPoint root = ui->canvasAnchor->mapTo(this, ui->canvasAnchor->geometry().center()) - viewCanvas.rect().center();
    root += viewCanvasOffset;
    painter.drawImage(root.x(), root.y(), viewCanvas, 0, 0, 0, 0);
}

void View::mousePressEvent(QMouseEvent* e)
{
    emit mousePressed(e);
}

void View::mouseReleaseEvent(QMouseEvent* e)
{
    emit mouseReleased(e);
}

void View::mouseMoveEvent(QMouseEvent* e)
{
    emit mouseMoved(e);
}

void View::on_addFrameButton_pressed()
{
    //TODO
    //add stuff to signal add frame
    emit pressedAddFrame();
    ui->frameslist->addItem("Frame " + QString:: number(frame.currentFrame.getIndex()));
}


void View::on_removeFrameButton_pressed()
{
    //TODO
    //add stuff to add signal frame delete
}

