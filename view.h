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
#include <QMessageBox>

QT_BEGIN_NAMESPACE
namespace Ui { class View; }
QT_END_NAMESPACE
// Authors
// Devin White
// Xuyen Nguyen
// Taylor Adamson
// Ansam Al Sharif
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
    void updateViewValue(Serialization::Key, QJsonValue);
    void openFileExplorer();
    void saveFileDialog(QByteArray);
    void saveExistingFile(QByteArray, QString);
    int animIndex = 0;
    bool startAnimate = false;
    bool loopAnim = false;
    bool animOnCanvas = false;
    void enableUiElements();
    void disableUiElements();
    void updatePreviewPanel(int index);
    bool actualSize = false;
    void setWidthHeightBoxValue(int height, int width);
    //find a better way
    int origFrameIndex;
    void openNewFile();

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
    void canvasOffsetChanged(QPoint);
    void pressedAddFrame(int atIndex, int oldIndex);
    void pressedRemoveFrame(int index);
    void colorSelected(QColor);
    void selectNewFrame(int newIndex, int oldIndex);
    void moveToNextFrame(int index);
    void updateCurrentEditor(int index);
    void loadedSerializedValues(QJsonObject);
    void setSaved(bool saved);
    void setFileName(QString fileName);
private slots:
    void on_addFrameButton_pressed();
    void on_removeFrameButton_pressed();
    void on_framesList_itemDoubleClicked(QListWidgetItem *item);
    void on_framesList_itemClicked(QListWidgetItem *item);
    void setColor(QTableWidgetItem *item);
    void updatePreviewOnFrameChange();
    void on_playButton_pressed();
    void playAnimation();
    void on_spriteHeight_valueChanged(int arg1);
    void on_spriteWidth_valueChanged(int arg1);
    void on_actualSizeToggle_toggled(bool checked);
    void on_loopButton_toggled(bool checked);
    void on_canvasAnimButton_toggled(bool checked);
};
#endif // VIEW_H
