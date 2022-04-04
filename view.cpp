#include "view.h"
#include "ui_view.h"
#include <QGraphicsPixmapItem>
#include <Qtimer>

View::View( Palette& palettePanel, Serialization& serialization, QWidget *parent)
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
    connect(&(frame.currentFrame), &Editor::serializeValue, &serialization, &Serialization::set);
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
    connect(&(frame.currentFrame),&Editor::updatePreview,this,&View::updatePreview);

    //set FPS box to only accept number between 0 - 100
    ui->fps->setValidator( new QIntValidator(0, 60, this) );

    // Set pen color
    connect(ui->paletteTable, &QTableWidget::itemClicked, this, &View::setColor);
    connect(this, &View::colorSelected, &(frame.currentFrame), &Editor::colorSelected);

    // Establish default values for various components
    // TODO: connect canvas methods to width and height sliders, move default values to serializer class
    ui->toolbar->setStyleSheet("QToolButton { margin: 5px; padding: 2px; }");
    ui->zoomSlider->setValue(serialization.getInt(Serialization::ZoomScale)); // canvas.setCanvasScale(8)
    frame.currentFrame.canvasWidthChanged(serialization.getInt(Serialization::Width));
    frame.currentFrame.canvasHeightChanged(serialization.getInt(Serialization::Height));
    palettePanel.paletteColumnsChanged(serialization.getInt(Serialization::PaletteColumnCount));
    palettePanel.paletteRowsChanged(serialization.getInt(Serialization::PaletteRowCount));

    ui->frameslist->addItem("Frame " + QString:: number(frame.currentFrame.getIndex()));
    ui->frameslist->setCurrentRow(0);
    frame.currentFrameIndex = 0;
    frame.frameNameCounter = 0;

    connect(ui->actionOpen, &QAction::triggered, &serialization, &Serialization::OpenFile);
    connect(ui->actionSaveAs, &QAction::triggered, &serialization, &Serialization::SaveAsFile);
    connect(ui->actionSave, &QAction::triggered, &serialization, &Serialization::SaveFile);
    connect(ui->actionNew, &QAction::triggered, &serialization, &Serialization::NewFile);
    connect(&serialization, &Serialization::openFileExplorer, this, &View::openFileExplorer);
    connect(&serialization, &Serialization::saveFileDialog, this, &View::saveFileDialog);
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
    updatePreview();
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
    frame.frameNameCounter++;
    int latestIndex = frame.frameNameCounter;
    emit pressedAddFrame(nextIndex, frame.currentFrameIndex);

    // Add new item to frameslist and select the added item
    ui->frameslist->insertItem(nextIndex , "Frame " + QString::number(latestIndex));
    ui->frameslist->setCurrentRow(nextIndex);
    frame.currentFrameIndex = nextIndex ;
//    ui->frameslist->addItem("Frame " + QString:: number(frame.currentFrame.getIndex()));

    // Update canvas view
    emit canvasAnchorChanged(calculateViewCanvasAnchor());
    frame.currentFrame.refreshCanvas();
}


void View::on_removeFrameButton_pressed()
{

    //removes from the list if total is above 1
    if(ui->frameslist->count() > 1)
    {
        //removes current row
        int index = ui->frameslist->currentRow();
        emit pressedRemoveFrame(index);
        //remove
        emit canvasAnchorChanged(calculateViewCanvasAnchor());
       ui->frameslist->takeItem(index);
       frame.currentFrameIndex = ui->frameslist->currentRow();
       //change this to signal to refresh
       frame.currentFrame.refreshCanvas();
    }

}


void View::on_frameslist_itemDoubleClicked(QListWidgetItem *item)
{
    //TODO
    // Change editor to current Frame
//   int index = ui->frameslist->row(item);
   int newIndex = ui->frameslist->currentRow();
   int oldIndex = frame.currentFrameIndex;
   frame.currentFrameIndex = newIndex;
   std::cout << "index of item: " << newIndex << ", current row selected: " << ui->frameslist->currentRow() << std::endl;
   emit selectNewFrame(newIndex, oldIndex);
   //remove
   emit canvasAnchorChanged(calculateViewCanvasAnchor());
   //change this to signal to refresh
   frame.currentFrame.refreshCanvas();
}


void View::on_frameslist_itemClicked(QListWidgetItem *item)
{
    updatePreview();
//    int index = ui->frameslist->row(item);

//    frame.currentFrameIndex = index;
//    std::cout << "index of item: " << index << ", current row selected: " << ui->frameslist->currentRow() << std::endl;
    //TODO
    //Change image in preview label to selected frame
    //(double click will actually change current frame to that frame)
}

void View::setColor(QTableWidgetItem *item) {
    QBrush brush = item->background();
    emit colorSelected(brush.color());
}

void View::updatePreview()
{
    frame.updateCurrentEditor();
    int index = ui->frameslist->currentRow();
    Editor currentPreview = frame.totalFrameVector[index];
    QImage image = currentPreview.getImage().scaledToHeight(ui->preview->height());
    QGraphicsPixmapItem *pixmapItem = new QGraphicsPixmapItem(QPixmap::fromImage(image));
    QGraphicsScene* scene = new QGraphicsScene;
    scene->addItem(pixmapItem);
    ui->preview->setScene(scene);
}
//Improve this. Rough test
void View::playAnimation()
{
    //disable elements
    ui->frameslist->setEnabled(false);
    ui->addFrameButton->setEnabled(false);
    ui->removeFrameButton->setEnabled(false);
    ui->playButton->setEnabled(false);
    ui->fps->setEnabled(false);

    //grabs number from fps box and sets it to proper time
    QString fps = ui->fps->text();
    int framesPerSecond = fps.toUInt();
    int fpstime = 1000/framesPerSecond;

    Editor currentPreview = frame.totalFrameVector[animIndex];
    QImage image = currentPreview.getImage().scaledToHeight(ui->preview->height());
    QGraphicsPixmapItem *pixmapItem = new QGraphicsPixmapItem(QPixmap::fromImage(image));
    QGraphicsScene* scene = new QGraphicsScene;
    scene->addItem(pixmapItem);
    ui->preview->setScene(scene);
    if(animIndex < frame.totalFrameVector.size() - 1 && startAnimate)
    {
        QTimer::singleShot(fpstime,this, &View::playAnimation);
        animIndex++;
    }
    else
    {
        ui->frameslist->setEnabled(true);
        ui->addFrameButton->setEnabled(true);
        ui->removeFrameButton->setEnabled(true);
        ui->playButton->setEnabled(true);
        ui->fps->setEnabled(true);
        startAnimate = false;
    }

}

void View::on_playButton_pressed()
{    //frame.updateCurrentEditor();
    animIndex = 0;
    startAnimate = true;
    Editor currentPreview = frame.totalFrameVector[animIndex];
    QImage image = currentPreview.getImage().scaledToHeight(ui->preview->height());
    QGraphicsPixmapItem *pixmapItem = new QGraphicsPixmapItem(QPixmap::fromImage(image));
    QGraphicsScene* scene = new QGraphicsScene;
    scene->addItem(pixmapItem);
    ui->preview->setScene(scene);
    playAnimation();
}

void View::openFileExplorer(){
    QFileDialog dialog(this);
    dialog.setNameFilter("Sprite Files (*.ssp)");
    dialog.setWindowTitle("Open Sprite");
    if (dialog.exec()){
        //TODO read the file
    }
}

void View::saveFileDialog(){
//    QFileDialog dialog(this);
//    dialog.setFileMode(QFileDialog::AnyFile);
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), "/sprite", tr("Sprite File (*.ssp"));
    if (fileName != ""){
        QFile file(QFileInfo(fileName).absoluteFilePath());
        if (file.open(QIODevice::WriteOnly)){
            //TODO add info to file here
            file.close();
        }
        else {
            QMessageBox::information(this, tr("Saving File"), tr("Could not save sprite file"));
        }
    }
}
