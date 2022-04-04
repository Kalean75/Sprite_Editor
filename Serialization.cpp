#include <Serialization.h>
#include <QDebug>

Serialization::Serialization(QObject* parent) : QObject(parent){

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
