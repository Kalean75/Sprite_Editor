#ifndef FRAME_H
#define FRAME_H

#include <QObject>
#include <QPainter>
#include "editor.h"
#include <iostream>

class Frame : public QObject
{
    Q_OBJECT

private:
   int  totalFrames;
public:
    explicit Frame(QObject *parent = nullptr);
    std::vector<Editor> totalFrameVector;
    //copy constructor
    Frame(const Frame& other);
    //destructor
    ~Frame();
    // = operator
    Frame& operator=(const Frame other);
    void startingFrameEditor();
    void setframeNumber(int number);
    int getframeNumber();
    Editor currentFrame;
    //signals?
    void addNewFrame();
    void removeOldFrame(int index);
    void selectNewFrame(int index);
    int gettotalFrames();
    void settotalFrames();



public slots:

signals:
    //change image on frame preview
    void changeImage(QPixmap);

};

#endif // FRAME_H
