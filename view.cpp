#include "view.h"
#include "ui_view.h"

View::View(PanelCanvas& canvas, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::View)
{
    ui->setupUi(this);
    toolActionGroup = new QActionGroup(ui->toolbar);
    foreach(QAction* action, ui->toolbar->actions())
    {
        toolActionGroup->addAction(action);
    }
    connect(ui->zoomSlider, &QSlider::valueChanged, &canvas, &PanelCanvas::setCanvasScale);
    connect(&canvas, &PanelCanvas::updateCanvasLabel, this, &View::updateCanvasLabel);
    // Establish default values for various components
    // TODO: connect canvas methods to width and height sliders, move default values to serializer class
    ui->toolbar->setStyleSheet("QToolButton { margin: 5px; padding: 2px; }");
    ui->zoomSlider->setValue(8); // canvas.setCanvasScale(8)
    canvas.setCanvasWidth(64);
    canvas.setCanvasHeight(64);
}

View::~View()
{
    delete ui;
}

void View::updateCanvasLabel(const QImage& canvas)
{
    ui->canvasLabel->setPixmap(QPixmap::fromImage(canvas));
}
