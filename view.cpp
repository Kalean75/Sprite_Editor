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
    connect(&editorPanel, &Editor::updateViewCanvas, this, &View::updateViewCanvas);
    connect(&palettePanel, &Palette::updateViewPalette, this, &View::updateViewPalette);
    connect(ui->zoomSlider, &QSlider::valueChanged, &editorPanel, &Editor::canvasScaleChanged);
    connect(this, &View::canvasAnchorChanged, &editorPanel, &Editor::canvasAnchorChanged);
    connect(this, &View::mousePressed, &editorPanel, &Editor::mousePressed);
    connect(this, &View::mouseReleased, &editorPanel, &Editor::mouseReleased);
    connect(this, &View::mouseMoved, &editorPanel, &Editor::mouseMoved);
    //Frame related
    connect(this,&View::pressedAddFrame,&frame,&Frame::addNewFrame);
    connect(this,&View::pressedRemoveFrame,&frame,&Frame::removeOldFrame);

    // Set pen color
    connect(ui->paletteTable, &QTableWidget::itemClicked, this, &View::setColor);
    connect(this, &View::colorSelected, &editorPanel, &Editor::colorSelected);

    // Establish default values for various components
    // TODO: connect canvas methods to width and height sliders, move default values to serializer class
    ui->toolbar->setStyleSheet("QToolButton { margin: 5px; padding: 2px; }");
    ui->zoomSlider->setValue(16); // canvas.setCanvasScale(8)
    editorPanel.canvasWidthChanged(32);
    editorPanel.canvasHeightChanged(32);
    palettePanel.paletteColumnsChanged(5);
    palettePanel.paletteRowsChanged(5);

    ui->frameslist->addItem("Frame " + QString:: number(frame.currentFrame.getIndex()));
}

View::~View()
{
    delete ui;
}

QPoint View::calculateViewCanvasAnchor()
{
    return ui->canvasAnchor->mapTo(this, ui->canvasAnchor->geometry().center());
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
    QPoint origin = (calculateViewCanvasAnchor() - viewCanvas.rect().center()) + viewCanvasOffset;
    painter.drawImage(origin.x(), origin.y(), viewCanvas, 0, 0, 0, 0);
}

void View::resizeEvent(QResizeEvent*)
{
    // First resize event won't get the correct layout geometry unless forcibly updated
    ui->leftLayout->invalidate();
    ui->leftLayout->activate();
    emit canvasAnchorChanged(calculateViewCanvasAnchor());
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
    //removes current row
    emit pressedRemoveFrame();
    ui->frameslist->takeItem(frame.currentFrame.getIndex());
}


void View::on_frameslist_itemDoubleClicked(QListWidgetItem *item)
{
    //TODO
    // Change editor to current Frame
}


void View::on_frameslist_itemClicked(QListWidgetItem *item)
{
    //TODO
    //Change image in preview label to selected frame
    //(double click will actually change current frame to that frame)
}

void View::setColor(QTableWidgetItem *item) {
    QColor color = item->backgroundColor();
    emit colorSelected(color);
}

