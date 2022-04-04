#include "frame.h"
#include <QtDebug>
#include <QFileDialog>


Frame::Frame(QObject *parent)
    : QObject{parent}
{
    totalFrames = 0;
    startingFrameEditor();
}

void Frame::startingFrameEditor()
{
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
void Frame:: addNewFrame(int atIndex, int oldIndex)
{
    //adds a frame to the vector and sets current frame to new one
//    totalFrameVector[currentFrame.getIndex()] = currentFrame;
    auto it = totalFrameVector.begin();
    totalFrameVector[oldIndex] = currentFrame;
    Editor nextFrame;
    currentFrame = nextFrame;
    totalFrameVector.insert(it+atIndex, currentFrame);
    totalFrames++;

}
void Frame:: removeOldFrame(int atIndex)
{
    auto it = totalFrameVector.begin();

    if (atIndex < totalFrameVector.size()-1)
    {
//        it = it + atIndex + 1;
        currentFrame = totalFrameVector[atIndex+1];
    }
    else
    {
        currentFrame = totalFrameVector[atIndex-1];
    }

    totalFrameVector.erase(it+atIndex);
    //TODO
    //ReDo this. doesn't work 100%
    //Change to remove any frame and not just last????
//    if(totalFrameVector.size() == 1)
//    {
//        //TODO
//        //Clear frame
//        return;
//    }
//    if(index == 0)
//    {
//        totalFrameVector.erase(totalFrameVector.begin());

//        for(int i = 0; i < (int)totalFrameVector.size(); i++)
//        {
//            totalFrameVector[i].setIndex(i);
//        }
//        currentFrame = totalFrameVector[0];
//        totalFrames--;
//                //TODO Update displayed frame
//        return;
//    }
//    if(index == (int)totalFrameVector.size() - 1)
//    {
//        currentFrame = totalFrameVector[index-1];
//        totalFrames--;
//        totalFrameVector.erase(totalFrameVector.end()-1);
//                //TODO Update displayed frame
//        return;
//    }
//    else
//    {
//        for(int i = index; i < (int)totalFrameVector.size() - 1; i++)
//        {
//            totalFrameVector[i] = totalFrameVector[i+1];
//            totalFrameVector[i].setIndex(i);
//        }
//        totalFrames--;
//        totalFrameVector.erase(totalFrameVector.end()-1);
//        //TODO Update displayed frame
//        return;
//    }
}

void Frame:: selectNewFrame(int newIndex, int oldIndex)
{
    totalFrameVector[oldIndex] = currentFrame;
    currentFrame = totalFrameVector[newIndex];
}

int Frame::gettotalFrames()
{
    return totalFrames;
}

void Frame::updateCurrentEditor()
{
    totalFrameVector[currentFrameIndex] = currentFrame;
}

void Frame::playSpriteAnimation()
{

}

