#ifndef VIEW_H
#define VIEW_H

#include <QMainWindow>
#include <QActionGroup>
#include <QPainter>
#include <QImage>
#include "panels/editor.h"
#include "panels/palette.h"
#include "panels/frame.h"
#include <QListWidget>
#include <QTableWidgetItem>
#include <Serialization.h>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class View; }
QT_END_NAMESPACE

class View : public QMainWindow
{
    Q_OBJECT
public:
    View(Palette& palettePanel, Serialization& serialization, QWidget *parent = nullptr);
    ~View();
private:
    Frame frame;
    Ui::View* ui;
    QActionGroup* toolActionGroup;
    QImage viewCanvas;
    QPoint viewCanvasOffset;
    QPoint calculateViewCanvasAnchor();
    int currentFrameIndex;
    void updateViewCanvas(const QImage&, QPoint);
    void updateViewPalette(const QVector<QString>&, QSize);

protected:
    void paintEvent(QPaintEvent*);
    void resizeEvent(QResizeEvent*);
    void mousePressEvent(QMouseEvent*);
    void mouseReleaseEvent(QMouseEvent*);
    void mouseMoveEvent(QMouseEvent*);
signals:
    void mousePressed(QMouseEvent*);
    void mouseReleased(QMouseEvent*);
    void mouseMoved(QMouseEvent*);
    void canvasAnchorChanged(QPoint);
    void pressedAddFrame(int atIndex, int oldIndex);
    void pressedRemoveFrame(int index);
    void colorSelected(QColor);
    void selectNewFrame(int newIndex, int oldIndex);
    void updateCurrentEditor(int index);
    void playSpriteAnimation();
private slots:
    void on_addFrameButton_pressed();
    void on_removeFrameButton_pressed();
    void on_frameslist_itemDoubleClicked(QListWidgetItem *item);
    void on_frameslist_itemClicked(QListWidgetItem *item);
    void setColor(QTableWidgetItem *item);
    void updatePreview();
    void playAnimation(int index);
    void on_playButton_pressed();
};
#endif // VIEW_H
