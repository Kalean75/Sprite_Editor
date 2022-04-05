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
   void setTotalFrames();
   void setframeNumber(int number);
public:
    explicit Frame(QObject *parent = nullptr);
    int frameNameCounter;
    std::vector<Editor> totalFrameVector;
    //copy constructor
    Frame(const Frame& other);
    //destructor
    ~Frame();
    // = operator
    Frame& operator=(const Frame other);
    int getframeNumber();
    Editor currentFrame;
    int gettotalFrames();
    void saveCurrentFrame();
    void resetState();
    int currentFrameIndex;

public slots:
    void addNewFrame(int atIndex, int oldIndex);
    void removeOldFrame(int index);
    void selectNewFrame(int newIndex, int oldIndex);
    void moveToNextFrame(int index);

signals:

};

#endif // FRAME_H
