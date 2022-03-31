#include "view.h"
#include "ui_view.h"

View::View(Editor& editorPanel, QWidget *parent)
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
    connect(this, &View::mousePressed, &editorPanel, &Editor::mousePressed);
    connect(this, &View::mouseReleased, &editorPanel, &Editor::mouseReleased);
    connect(this, &View::mouseMoved, &editorPanel, &Editor::mouseMoved);
    // Establish default values for various components
    // TODO: connect canvas methods to width and height sliders, move default values to serializer class
    ui->toolbar->setStyleSheet("QToolButton { margin: 5px; padding: 2px; }");
    ui->zoomSlider->setValue(8); // canvas.setCanvasScale(8)
    editorPanel.canvasWidthChanged(64);
    editorPanel.canvasHeightChanged(64);
}

View::~View()
{
    delete ui;
}

void View::updateViewCanvas(const QImage& canvas, QPoint offset)
{
    viewCanvas = canvas;
    viewCanvasOffset = offset;
    repaint();
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
