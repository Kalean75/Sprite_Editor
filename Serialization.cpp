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
    config.insert(configDefaults.at(static_cast<int>(k)).first, v);
}

void Serialization::SaveAsFile(bool checked){
    emit saveFileDialog();
}

void Serialization::NewFile(bool checked){

}

void Serialization::OpenFile(bool checked){
    emit openFileExplorer();
}

void Serialization::SaveFile(bool checked){

}
