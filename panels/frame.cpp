#include "frame.h"
#include <QtDebug>
#include <QFileDialog>

Frame::Frame(QObject *parent)
    : QObject{parent}
{
    totalFrames = 0;
}

void Frame::startingFrameEditor()
{
    currentFrame.setIndex(totalFrames);
    Editor nextFrame;
    currentFrame = nextFrame;
    totalFrameVector.push_back(currentFrame);
    totalFrames++;
}
//copy constructor
Frame::Frame(const Frame& other)
{

}
//destructor
Frame::~Frame()
{

}
// = operator
Frame& Frame::operator=(const Frame other)
{

}
void Frame:: addNewFrame()
{
    totalFrames++;
    totalFrameVector[currentFrame.getIndex()] = currentFrame;
    Editor nextFrame;
    currentFrame = nextFrame;
    currentFrame.setIndex(totalFrames);
    totalFrameVector.push_back(nextFrame);
    //keep selected colors/brushes?
}
void Frame:: removeOldFrame()
{
    //TODO
    //remove frame from list
}
