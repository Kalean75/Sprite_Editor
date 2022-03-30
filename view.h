#ifndef VIEW_H
#define VIEW_H

#include <QMainWindow>
#include <QActionGroup>
#include <QImage>

QT_BEGIN_NAMESPACE
namespace Ui { class View; }
QT_END_NAMESPACE

class View : public QMainWindow
{
    Q_OBJECT

public:
    View(QWidget *parent = nullptr);
    ~View();

private:
    const int canvasDefaultScale = 8;
    const int canvasCheckerboardSize = 8;
    const int canvasDefaultSize = 64;
    int canvasScale = canvasDefaultScale;
    QSize canvasSize = QSize(canvasDefaultSize, canvasDefaultSize);
    QImage canvas;
    QActionGroup* toolActionGroup;
    Ui::View *ui;
};
#endif // VIEW_H
