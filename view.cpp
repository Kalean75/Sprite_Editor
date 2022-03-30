#include "view.h"
#include "ui_view.h"

View::View(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::View)
{
    ui->setupUi(this);
    toolActionGroup = new QActionGroup(ui->toolbar);
    canvas = QImage(canvasSize, QImage::Format_ARGB32_Premultiplied).scaled(canvasSize * canvasScale);
    canvas.fill(Qt::transparent);
    for (int x = 0; x < canvas.width(); x++)
    {
        for (int y = 0; y < canvas.height(); y++)
        {
            if ((x / canvasCheckerboardSize + y / canvasCheckerboardSize) % 2 == 0)
                canvas.setPixel(x, y, QColor(Qt::lightGray).rgb());
            else
                canvas.setPixel(x, y, QColor(Qt::white).rgb());
        }
    }
    ui->canvasLabel->setPixmap(QPixmap::fromImage(canvas));
    foreach(QAction* action, ui->toolbar->actions())
    {
        toolActionGroup->addAction(action);
    }
    ui->actionPencil->setChecked(true);
}

View::~View()
{
    delete ui;
}
