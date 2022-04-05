#include <Serialization.h>
#include <QDebug>

Serialization::Serialization(QObject* parent) : QObject(parent)
{
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

void Serialization::SaveAsFile(bool)
{
    QJsonDocument document;
    document.setObject(config);
    emit saveFileDialog(document.toJson(QJsonDocument::Compact));
}

void Serialization::NewFile(bool)
{

}

void Serialization::OpenFile(bool){
    emit openFileExplorer();
}

void Serialization::SaveFile(bool){

}

void Serialization::loadedSerializedValues(QJsonObject loaded)
{
    config = loaded;
    for (int i = 0; i < (int) configDefaults.size(); i++)
    {
        emit updateViewValue(static_cast<Key>(i), loaded.value(configDefaults.at(i).first));
    }
}
