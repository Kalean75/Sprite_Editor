#include "view.h"
#include "ui_view.h"

View::View( Palette& palettePanel, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::View)
{
    //replace (frame.currentFrame) to editorPanel to change it to old way
    ui->setupUi(this);
    toolActionGroup = new QActionGroup(ui->toolbar);
    foreach(QAction* action, ui->toolbar->actions())
    {
        toolActionGroup->addAction(action);
        connect(action, &QAction::triggered, &(frame.currentFrame), &Editor::toolSelected);
    }
    connect(&(frame.currentFrame), &Editor::updateViewCanvas, this, &View::updateViewCanvas);
    connect(&palettePanel, &Palette::updateViewPalette, this, &View::updateViewPalette);
    connect(ui->zoomSlider, &QSlider::valueChanged, &(frame.currentFrame), &Editor::canvasScaleChanged);
    connect(this, &View::canvasAnchorChanged, &(frame.currentFrame), &Editor::canvasAnchorChanged);
    connect(this, &View::mousePressed, &(frame.currentFrame), &Editor::mousePressed);
    connect(this, &View::mouseReleased, &(frame.currentFrame), &Editor::mouseReleased);
    connect(this, &View::mouseMoved, &(frame.currentFrame), &Editor::mouseMoved);
    //Frame related
    connect(this,&View::pressedAddFrame,&frame,&Frame::addNewFrame);
    connect(this,&View::pressedRemoveFrame,&frame,&Frame::removeOldFrame);
    connect(this,&View::selectNewFrame,&frame,&Frame::selectNewFrame);

    // Set pen color
    connect(ui->paletteTable, &QTableWidget::itemClicked, this, &View::setColor);
    connect(this, &View::colorSelected, &(frame.currentFrame), &Editor::colorSelected);

    // Establish default values for various components
    // TODO: connect canvas methods to width and height sliders, move default values to serializer class
    ui->toolbar->setStyleSheet("QToolButton { margin: 5px; padding: 2px; }");
    ui->zoomSlider->setValue(16); // canvas.setCanvasScale(8)
    //frame.currentFrame.canvasWidthChanged(32);
    //frame.currentFrame.canvasHeightChanged(32);
    palettePanel.paletteColumnsChanged(5);
    palettePanel.paletteRowsChanged(5);

    ui->frameslist->addItem("Frame " + QString:: number(frame.currentFrame.getIndex()));
    ui->frameslist->setCurrentRow(0);
    currentFrameIndex = 0;
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
    int nextIndex = ui->frameslist->currentRow() + 1;
    int latestIndex = ui->frameslist->count();
    emit pressedAddFrame(nextIndex, currentFrameIndex);

    // Add new item to frameslist and select the added item
    ui->frameslist->insertItem(nextIndex , "Frame " + QString::number(latestIndex));
    ui->frameslist->setCurrentRow(nextIndex );
    currentFrameIndex = nextIndex ;
//    ui->frameslist->addItem("Frame " + QString:: number(frame.currentFrame.getIndex()));

    // Update canvas view
    emit canvasAnchorChanged(calculateViewCanvasAnchor());
    frame.currentFrame.refreshCanvas();
}


void View::on_removeFrameButton_pressed()
{
    //removes current row
    int index = ui->frameslist->currentRow();
    emit pressedRemoveFrame(index);
    //remove
    emit canvasAnchorChanged(calculateViewCanvasAnchor());
    //removes from the list if total is above 1
    if(ui->frameslist->count() > 1)
    {
       ui->frameslist->takeItem(index);
    }
    //change this to signal to refresh
       frame.currentFrame.refreshCanvas();
}


void View::on_frameslist_itemDoubleClicked(QListWidgetItem *item)
{
    //TODO
    // Change editor to current Frame
//   int index = ui->frameslist->row(item);
   int newIndex = ui->frameslist->currentRow();
   int oldIndex = currentFrameIndex;
   currentFrameIndex = newIndex;
   std::cout << "index of item: " << newIndex << ", current row selected: " << ui->frameslist->currentRow() << std::endl;
   emit selectNewFrame(newIndex, oldIndex);
   //remove
   emit canvasAnchorChanged(calculateViewCanvasAnchor());
   //change this to signal to refresh
   frame.currentFrame.refreshCanvas();
}


void View::on_frameslist_itemClicked(QListWidgetItem *item)
{
//    int index = ui->frameslist->row(item);
//    int index = ui->frameslist->currentRow();
//    currentFrameIndex = index;
//    std::cout << "index of item: " << index << ", current row selected: " << ui->frameslist->currentRow() << std::endl;
    //TODO
    //Change image in preview label to selected frame
    //(double click will actually change current frame to that frame)
}

void View::setColor(QTableWidgetItem *item) {
    //QColor color = item->backgroundColor();
    //emit colorSelected(color);
}

