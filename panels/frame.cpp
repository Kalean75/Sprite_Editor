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
    //totalFrames++;
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
    //adds a frame to the vector and sets current frame to new one
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
    //Change to remove any frame and not just last????
    if(totalFrames > 0)
    {
        totalFrames--;
        currentFrame = totalFrameVector[totalFrames];
        totalFrameVector.pop_back();
        /*for(Editor frame: totalFrameVector)
        {
            frame.setIndex(frame.getIndex() - 1);
        }*/
    }
    //TODO
    //remove frame from list
}
