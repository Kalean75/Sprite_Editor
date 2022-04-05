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
    int currentFrameIndex;
    int frameNameCounter;
    std::vector<Editor> totalFrameVector;
    //copy constructor
    Frame(const Frame& other);
    //destructor
    ~Frame();
    // = operator
    Frame& operator=(const Frame other);
    void setframeNumber(int number);
    int getframeNumber();
    Editor currentFrame;
    //signals?
    void addNewFrame(int atIndex, int oldIndex);
    void removeOldFrame(int index);
    void selectNewFrame(int newIndex, int oldIndex);
    int gettotalFrames();
    void settotalFrames();
    void updateCurrentEditor();

public slots:


signals:

};

#endif // FRAME_H
