#include "frame.h"
#include <QtDebug>
#include <QFileDialog>
// Authors
// Devin White
// Xuyen Nguyen
// Taylor Adamson
// Ansam Al Sharif
//constructor
//sets initial frames
Frame::Frame(QObject *parent)
    : QObject{parent}
{
    totalFrames = 0;
    Editor nextFrame;
    currentFrame = nextFrame;
    currentFrame.setIndex(totalFrames);
    totalFrameVector.push_back(nextFrame);
    totalFrames++;
}

//copy constructor
Frame::Frame(const Frame& other)
{
    totalFrames = other.totalFrames;
    totalFrameVector = other.totalFrameVector;

    //TODO
    //insert more if needed
}
//destructor
Frame::~Frame()
{
    totalFrameVector.clear();
    totalFrames = 0;
}
// = operator
Frame& Frame::operator=(Frame other)
{
    std::swap(totalFrames, other.totalFrames);
    std::swap(totalFrameVector, other.totalFrameVector);
    //Add in more as needed
    return *this;
}
//adds a new frame to the vector of frames(editors) and sets current frame to the new one
void Frame:: addNewFrame(int atIndex, int oldIndex)
{
    //adds a frame to the vector and sets current frame to new one
    auto it = totalFrameVector.begin();
    if (totalFrameVector.size() > 0)
    {
        totalFrameVector[oldIndex] = currentFrame;
    }
    for (int i = atIndex; i < (int) totalFrameVector.size(); i++)
    {
        currentFrame = totalFrameVector.at(i);
        currentFrame.setIndex(i + 1);
        emit serializeValue(Serialization::Frames, currentFrame.getSerializedFrame());
    }
    Editor nextFrame;
    currentFrame = nextFrame;
    totalFrameVector.insert(it+atIndex, currentFrame);
    totalFrames++;
    currentFrame.setIndex(atIndex);

}
//removes a frame from the vector of frames(editors)
void Frame:: removeOldFrame(int atIndex)
{
    auto it = totalFrameVector.begin();

    if (atIndex < totalFrameVector.size() -1)
    {
        currentFrame = totalFrameVector[atIndex+1];
    }
    else
    {
        currentFrame = totalFrameVector[atIndex-1];
    }

    totalFrameVector.erase(it+atIndex);
}

//selects a new frame
void Frame:: selectNewFrame(int newIndex, int oldIndex)
{
    totalFrameVector[oldIndex] = currentFrame;
    currentFrame = totalFrameVector[newIndex];
}
void Frame::moveToNextFrame(int index)
{
    currentFrame = totalFrameVector[index];
}
//getter for total frames
//returns the total number of frames in the vector
int Frame::gettotalFrames()
{
    return totalFrames;
}
//saves the current editor window into the frame vector
void Frame::saveCurrentFrame()
{
    totalFrameVector[currentFrameIndex] = currentFrame;
}

void Frame::resetState()
{
    totalFrames = 0;
    totalFrameVector.clear();
    currentFrameIndex = 0;
    frameNameCounter = 0;
}

