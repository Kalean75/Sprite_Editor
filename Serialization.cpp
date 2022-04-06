#include <Serialization.h>
#include <QDebug>
// Authors
// Devin White
// Xuyen Nguyen
// Taylor Adamson
// Ansam Al Sharif
Serialization::Serialization(QObject* parent) : QObject(parent)
{
    configDefaults.append(qMakePair(QString("width"), QJsonValue(32)));
    configDefaults.append(qMakePair(QString("height"), QJsonValue(32)));
    configDefaults.append(qMakePair(QString("frames"), QJsonValue(QJsonObject())));
    configDefaults.append(qMakePair(QString("numberOfFrames"), QJsonValue(1)));
    configDefaults.append(qMakePair(QString("numberOfPaletteColumns"), QJsonValue(5)));
    configDefaults.append(qMakePair(QString("numberOfPaletteRows"), QJsonValue(5)));
    configDefaults.append(qMakePair(QString("zoomScale"), QJsonValue(16)));
    for (int i = 0; i < (int) configDefaults.size(); i++)
    {
        set(static_cast<Key>(i), configDefaults.at(i).second);
    }
}

QJsonValue Serialization::get(Key k)
{
    return config.value(configDefaults.at(static_cast<int>(k)).first);
}

int Serialization::getInt(Key k)
{
    return get(k).toInt();
}

void Serialization::set(Key k, QJsonValue v)
{
    QString resolvedKey = configDefaults.at(static_cast<int>(k)).first;
    if (k == Frames)
    {
        QJsonArray frameData = v.toArray();
        QJsonObject frameObject = get(Frames).toObject();
        // In the editor panel, the frame index is temporarily put in the frame array since a QPair cannot be passed to set
        int frameIndex = frameData.takeAt(0).toInt();
        frameObject.insert(QString("frame").append(QString::number(frameIndex)), frameData);
        config.insert(resolvedKey, frameObject);
    }
    else
    {
        config.insert(resolvedKey, v);
    }
}

void Serialization::saveAsFile(bool)
{
    QJsonDocument document;
    document.setObject(config);
    saved = true;
    emit saveFileDialog(document.toJson(QJsonDocument::Compact));
}

void Serialization::newFile(bool)
{
    if (saved == true){
        //open a new file
    }
    else {
        QMessageBox msgBox;
        msgBox.setText("Open a New Sprite");
        msgBox.setInformativeText("The current sprite has not been saved. Are you sure you want to open a new sprite?");
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::Yes);
        int result = msgBox.exec();
        switch(result){
        case QMessageBox::Yes:
            // open new file
            emit openNewFile();
            break;
        case QMessageBox::No:
            // do nothing
            break;
        default:
            break;
        }
    }
}

void Serialization::openFile(bool){
    emit openFileExplorer();
}

void Serialization::saveFile(bool){
}

void Serialization::loadedSerializedValues(QJsonObject loaded)
{
    config = QJsonObject(loaded);
    for (int i = 0; i < (int) configDefaults.size(); i++)
    {
        emit updateViewValue(static_cast<Key>(i), loaded.value(configDefaults.at(i).first));
    }
}
