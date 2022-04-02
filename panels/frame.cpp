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
void Frame:: addNewFrame()
{
    //adds a frame to the vector and sets current frame to new one
    totalFrameVector[currentFrame.getIndex()] = currentFrame;
    Editor nextFrame;
    currentFrame = nextFrame;
    currentFrame.setIndex(totalFrames);
    totalFrameVector.push_back(nextFrame);
    totalFrames++;
    //TODO
    //update view
    //keep selected colors/brushes?
}
void Frame:: removeOldFrame(int index)
{
    //Change to remove any frame and not just last????
    if(totalFrameVector.size() == 1)
    {
        //TODO
        //Clear frame
    }
    if(index == 0)
    {
        totalFrameVector.erase(totalFrameVector.begin());

        for(int i = 0; i < totalFrameVector.size(); i++)
        {
            totalFrameVector[i].setIndex(i);
        }
        currentFrame = totalFrameVector[0];
        totalFrames--;
                //TODO Update displayed frame
        return;
    }
    if(index == totalFrameVector.size() - 1)
    {
        currentFrame = totalFrameVector[index-1];
        totalFrames--;
        totalFrameVector.erase(totalFrameVector.end()-1);
                //TODO Update displayed frame
        return;
    }
    else
    {
        for(int i = index; i < totalFrameVector.size() - 1; i++)
        {
            totalFrameVector[i] = totalFrameVector[i+1];
            totalFrameVector[i].setIndex(i);
        }
        totalFrames--;
        totalFrameVector.erase(totalFrameVector.end()-1);
        //TODO Update displayed frame
        return;
    }
}

void Frame:: selectNewFrame(int index)
{
    //if selecting a frame other than current
    if(currentFrame.getIndex() != index)
    {
        //save current
        totalFrameVector[currentFrame.getIndex()] = currentFrame;
        //set current frame to index frame
        currentFrame = totalFrameVector[index];
        //debugging. Remove
        std::cout<<"index " << index<<std::endl;
        std::cout<<"CurrentFrame index " << currentFrame.getIndex()<<std::endl;
        currentFrame.refreshCanvas();

        //TODO refresh displayed frame with image of new frame.
    }
    //Debugging. Remove
    else
    {
       std::cout<<"CurrentFrame index " << currentFrame.getIndex()<<std::endl;
    }
}

int Frame:: gettotalFrames()
{
    return totalFrames;
}

