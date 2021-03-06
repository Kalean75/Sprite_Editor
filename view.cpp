#include "view.h"
#include "ui_view.h"
#include <QGraphicsPixmapItem>
#include <QTimer>
#include <QTextStream>
// Authors
// Devin White
// Xuyen Nguyen
// Taylor Adamson
// Ansam Al Sharif

View::View( Palette& palettePanel, Serialization& serialization, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::View)
{
    ui->setupUi(this);
    toolActionGroup = new QActionGroup(ui->toolbar);
    foreach(QAction* action, ui->toolbar->actions())
    {
        toolActionGroup->addAction(action);
        connect(action, &QAction::triggered, &(frame.currentFrame), &Editor::toolSelected);
    }
    connect(&(frame.currentFrame), &Editor::updateViewCanvas, this, &View::updateViewCanvas);
    connect(&(frame.currentFrame), &Editor::serializeValue, &serialization, &Serialization::set);
    connect(&frame, &Frame::serializeValue, &serialization, &Serialization::set);
    connect(&palettePanel, &Palette::updateViewPalette, this, &View::updateViewPalette);
    connect(ui->zoomSlider, &QSlider::valueChanged, &(frame.currentFrame), &Editor::canvasScaleChanged);
    connect(this, &View::canvasAnchorChanged, &(frame.currentFrame), &Editor::canvasAnchorChanged);
    connect(this, &View::canvasOffsetChanged, &(frame.currentFrame), &Editor::canvasOffsetChanged);
    connect(this, &View::mousePressed, &(frame.currentFrame), &Editor::mousePressed);
    connect(this, &View::mouseReleased, &(frame.currentFrame), &Editor::mouseReleased);
    connect(this, &View::mouseMoved, &(frame.currentFrame), &Editor::mouseMoved);

    //Frame related connections
    connect(this,&View::pressedAddFrame,&frame,&Frame::addNewFrame);
    connect(this,&View::pressedRemoveFrame,&frame,&Frame::removeOldFrame);
    connect(this,&View::selectNewFrame,&frame,&Frame::selectNewFrame);
    connect(this,&View::moveToNextFrame,&frame,&Frame::moveToNextFrame);
    connect(&(frame.currentFrame),&Editor::updatePreview,this,&View::updatePreviewOnFrameChange);

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

    ui->framesList->addItem("Frame " + QString:: number(frame.currentFrame.getIndex()));
    ui->framesList->setCurrentRow(0);
    frame.currentFrameIndex = 0;
    frame.frameNameCounter = 0;

    connect(ui->actionOpen, &QAction::triggered, &serialization, &Serialization::openFile);
    connect(ui->actionSaveAs, &QAction::triggered, &serialization, &Serialization::saveAsFile);
    connect(ui->actionSave, &QAction::triggered, &serialization, &Serialization::saveFile);
    connect(ui->actionNew, &QAction::triggered, &serialization, &Serialization::newFile);
    connect(&serialization, &Serialization::openFileExplorer, this, &View::openFileExplorer);
    connect(&serialization, &Serialization::saveFileDialog, this, &View::saveFileDialog);
    connect(&serialization, &Serialization::saveExistingFile, this, &View::saveExistingFile);
    connect(&serialization, &Serialization::updateViewValue, this, &View::updateViewValue);
    connect(this, &View::loadedSerializedValues, &serialization, &Serialization::loadedSerializedValues);
    connect(&serialization, &Serialization::openNewFile, this, &View::openNewFile);
    connect(this, &View::setSaved, &serialization, &Serialization::setSaved);
    connect(this, &View::setFileName, &serialization, &Serialization::setFileName);
}

View::~View()
{
    delete ui;
}
//calculates the anchor for the canvas used in drawing
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

void View::updateViewValue(Serialization::Key k, QJsonValue v)
{
    switch(k)
    {
    case Serialization::Frames:
    {
        QJsonObject frameObject = v.toObject();
        frame.resetState();
        ui->framesList->clear();
        for (int i = 0; i < frameObject.size(); i++)
        {
            QString frameKey = QString("frame").append(QString::number(i));
            on_addFrameButton_pressed();
            frame.currentFrame.loadSerializedFrame(frameObject.value(frameKey).toArray());
        }
    }
        break;
    case Serialization::ZoomScale:
        ui->zoomSlider->setValue(v.toInt());
        break;
    default:
        break;
    }
}
//handles paint Events
void View::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    QPoint origin = (calculateViewCanvasAnchor() - viewCanvas.rect().center()) + viewCanvasOffset;
    painter.drawImage(origin.x(), origin.y(), viewCanvas, 0, 0, 0, 0);
    updatePreviewOnFrameChange();
}
//handles resize events
void View::resizeEvent(QResizeEvent*)
{
    // First resize event won't get the correct layout geometry unless forcibly updated
    ui->leftLayout->invalidate();
    ui->leftLayout->activate();
    emit canvasAnchorChanged(calculateViewCanvasAnchor());
}
//handles mouse pressed events
void View::mousePressEvent(QMouseEvent* e)
{
    emit mousePressed(e);
}

//handles mouse released events
void View::mouseReleaseEvent(QMouseEvent* e)
{
    emit mouseReleased(e);
}

//handles mouse move events
void View::mouseMoveEvent(QMouseEvent* e)
{
    emit mouseMoved(e);
}

//when clicked, adds a new frame
void View::on_addFrameButton_pressed()
{
    //TODO
    //add stuff to signal add frame
    int nextIndex = ui->framesList->currentRow() + 1;
    frame.frameNameCounter++;
    int latestIndex = frame.frameNameCounter;
    QSize previousSize = QSize(frame.currentFrame.getWidth(), frame.currentFrame.getHeight());
    emit pressedAddFrame(nextIndex, frame.currentFrameIndex);

    // Add new item to frameslist and select the added item
    ui->framesList->insertItem(nextIndex , "Frame " + QString::number(latestIndex));
    ui->framesList->setCurrentRow(nextIndex);
    frame.currentFrameIndex = nextIndex ;
    //    ui->frameslist->addItem("Frame " + QString:: number(frame.currentFrame.getIndex()));

    // Update canvas view
    emit canvasAnchorChanged(calculateViewCanvasAnchor());
    emit canvasOffsetChanged(viewCanvasOffset);
    frame.currentFrame.canvasWidthChanged(previousSize.width());
    frame.currentFrame.canvasHeightChanged(previousSize.height());
    //refactor
    setWidthHeightBoxValue(frame.currentFrame.getHeight(),frame.currentFrame.getWidth());
}

//when clicked, removes the selected frame
void View::on_removeFrameButton_pressed()
{

    //removes from the list if total is above 1
    if(ui->framesList->count() > 1)
    {
        //removes current row
        int index = ui->framesList->currentRow();
        emit pressedRemoveFrame(index);
        //remove
        emit canvasAnchorChanged(calculateViewCanvasAnchor());
        emit canvasOffsetChanged(viewCanvasOffset);
        ui->framesList->takeItem(index);
        frame.currentFrameIndex = ui->framesList->currentRow();
        //change this to signal to refresh
        frame.currentFrame.refreshCanvas();
        setWidthHeightBoxValue(frame.currentFrame.getHeight(),frame.currentFrame.getWidth());
    }

}

//when frame in frame list is double clicked, make that frame the active frame
void View::on_framesList_itemDoubleClicked(QListWidgetItem *item)
{
    // Change editor to current Frame
    int newIndex = ui->framesList->currentRow();
    int oldIndex = frame.currentFrameIndex;
    frame.currentFrameIndex = newIndex;
    //std::cout << "index of item: " << newIndex << ", current row selected: " << ui->frameslist->currentRow() << std::endl;
    emit selectNewFrame(newIndex, oldIndex);
    //remove
    emit canvasAnchorChanged(calculateViewCanvasAnchor());
    emit canvasOffsetChanged(viewCanvasOffset);
    //change this to signal to refresh
    frame.currentFrame.refreshCanvas();
    setWidthHeightBoxValue(frame.currentFrame.getHeight(),frame.currentFrame.getWidth());
}

//when item in frames list is clicked, displays that frame in preview
void View::on_framesList_itemClicked(QListWidgetItem *item)
{
    updatePreviewOnFrameChange();
}
void View::setWidthHeightBoxValue(int height, int width)
{
    ui->spriteWidth->setValue(width);
    ui->spriteHeight->setValue(height);
}
//sets color of brush/paintbucket to color selected in pallette
void View::setColor(QTableWidgetItem *item)
{
    QBrush brush = item->background();
    emit colorSelected(brush.color());
}

//updates the preview panel when swapping active frames
void View::updatePreviewOnFrameChange()
{
    if(!startAnimate)
    {
        frame.saveCurrentFrame();
        int index = ui->framesList->currentRow();
        updatePreviewPanel(index);
    }
}

//enables various UI elements
void View::enableUiElements()
{
    ui->framesList->setEnabled(true);
    ui->addFrameButton->setEnabled(true);
    ui->removeFrameButton->setEnabled(true);
    ui->playButton->setEnabled(true);
    ui->fps->setEnabled(true);
    ui->actionNew->setEnabled(true);
    ui->actionSaveAs->setEnabled(true);
    ui->actionOpen->setEnabled(true);
    ui->spriteHeight->setEnabled(true);
    ui->spriteWidth->setEnabled(true);
}
//disables Various UI elements
void View::disableUiElements()
{
    ui->framesList->setEnabled(false);
    ui->addFrameButton->setEnabled(false);
    ui->removeFrameButton->setEnabled(false);
    ui->playButton->setEnabled(false);
    ui->fps->setEnabled(false);
    ui->actionNew->setEnabled(false);
    ui->actionSaveAs->setEnabled(false);
    ui->actionOpen->setEnabled(false);
    ui->spriteHeight->setEnabled(false);
    ui->spriteWidth->setEnabled(false);
}

//when play button is preseed, begins animation
void View::on_playButton_pressed()
{
    //set displayed frame index to the first index(0)
    animIndex = 0;
    if(animOnCanvas)
    {
        origFrameIndex = frame.currentFrameIndex;
    }
    //start animation
    startAnimate = true;
    //update preview to first frame
    updatePreviewPanel(0);
    //disable elements
    disableUiElements();
    playAnimation();
}
//plays the sprite animation in the preview panel
//Improve this. Rough test
void View::playAnimation()
{

    //grabs number from fps box and sets it to proper time
    QString fps = ui->fps->text();
    int framesPerSecond = fps.toUInt();
    //frames per second calculated by 1 second(1000 ms)/frames
    int fpstime = 1000/framesPerSecond;
    //update Canvas View
    if(animOnCanvas)
    {
        emit moveToNextFrame(animIndex);
        frame.currentFrame.refreshCanvas();
    }
    //update preview panel
    updatePreviewPanel(animIndex);

    if((uint) animIndex < frame.totalFrameVector.size() - 1 && startAnimate)
    {
        QTimer::singleShot(fpstime,this, &View::playAnimation);
        animIndex++;
    }
    else
    {
        if(!loopAnim)
        {
            if(animOnCanvas)
            {
                emit moveToNextFrame(origFrameIndex);
                frame.currentFrame.refreshCanvas();
            }
            enableUiElements();
            startAnimate = false;
        }
        else
        {
            animIndex = 0;
            QTimer::singleShot(fpstime,this, &View::playAnimation);
        }
    }

}

//updates the displayed image within the preview panel
//used for animation and previewing other frames
void View::updatePreviewPanel(int index)
{
    Editor currentPreview = frame.totalFrameVector[index];
    QImage image = currentPreview.getImage().scaledToHeight(ui->preview->height());;
    if(actualSize)
    {
        image = currentPreview.getImage().scaled(frame.currentFrame.getWidth(), frame.currentFrame.getHeight());
    }
    QGraphicsPixmapItem *pixmapItem = new QGraphicsPixmapItem(QPixmap::fromImage(image));
    QGraphicsScene* scene = new QGraphicsScene;
    scene->addItem(pixmapItem);
    ui->preview->setScene(scene);
}

void View::openFileExplorer(){
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "/home/sprite", tr("Sprite File (*.ssp)"));
    if (!fileName.isEmpty()){
        QFile file(QFileInfo(fileName).absoluteFilePath());
        if (file.open(QIODevice::ReadOnly)){
            QByteArray bytes = file.readAll();
            QJsonParseError jsonError;
            QJsonDocument document = QJsonDocument::fromJson(bytes, &jsonError);
            if (jsonError.error == QJsonParseError::NoError && document.isObject())
            {
                emit loadedSerializedValues(document.object());
            }
            file.close();
        }
        else {
            QMessageBox::information(this, tr("Opening File"), tr("Could not open sprite file"));
        }
    }
}

void View::saveFileDialog(QByteArray jsonBytes){
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), "/home/sprite", tr("Sprite File (*.ssp)"));
    if (!fileName.isEmpty() && !fileName.isNull())
    {
        QFile file(QFileInfo(fileName).absoluteFilePath());
        if (file.open(QIODevice::WriteOnly))
        {
            QTextStream iStream( &file );
            iStream << jsonBytes;
            file.close();
            emit setFileName(fileName);
        }
        else
        {
            QMessageBox::information(this, tr("Saving File"), tr("Could not save sprite file"));
        }
    }
    else {
        emit setSaved(false);
    }
}

void View::saveExistingFile(QByteArray jsonBytes, QString fileName)
{
    if (!fileName.isEmpty() && !fileName.isNull())
    {
        QFile file(QFileInfo(fileName).absoluteFilePath());
        if (file.open(QIODevice::WriteOnly))
        {
            QTextStream iStream( &file );
            iStream << jsonBytes;
            file.close();
            emit setFileName(fileName);
        }
        else
        {
            QMessageBox::information(this, tr("Saving File"), tr("Could not save sprite file"));
        }
    }
    else {
        emit setSaved(false);
    }
}

//update height of sprite when value of height box is changed
void View::on_spriteHeight_valueChanged(int height)
{
    int newHeight = height;
    frame.currentFrame.canvasHeightChanged(newHeight);
}

//update width of sprite when value of width box is changed
void View::on_spriteWidth_valueChanged(int width)
{
    int newWidth = width;
    frame.currentFrame.canvasWidthChanged(newWidth);
}


void View::on_actualSizeToggle_toggled(bool checked)
{
    if(checked)
    {
        actualSize = true;
    }
    else
    {
        actualSize = false;
    }
}

void View::openNewFile(){
    //TODO open a new file
    ui->canvasAnimButton->setChecked(false);
    ui->loopButton->setChecked(false);
    ui->actualSizeToggle->setChecked(false);
    ui->framesList->clear();
    for(int index = 0; index < ui->framesList->count(); index++)
    {
        ui->framesList->takeItem(index);
    }
    frame.resetState();
    on_addFrameButton_pressed();
    ui->spriteHeight->setValue(32);
    ui->spriteWidth->setValue(32);

}


void View::on_loopButton_toggled(bool checked)
{
    if(checked)
    {
        loopAnim = true;
    }
    else
    {
        loopAnim = false;
    }
}


void View::on_canvasAnimButton_toggled(bool checked)
{
    if(checked)
    {
        animOnCanvas = true;
    }
    else
    {
        animOnCanvas = false;
    }
}

