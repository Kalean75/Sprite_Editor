#ifndef SERIALIZATION_H
#define SERIALIZATION_H
#include <QString>
#include <QObject>
#include <QFileDialog>

class Serialization : public QObject {
    Q_OBJECT
public:
    explicit Serialization(QObject *parent = nullptr);
public slots:
    void SaveAsFile(bool checked);
    void OpenFile(bool checked);
    void NewFile(bool checked);
    void SaveFile(bool checked);
signals:
    void openFileExplorer();
    void saveFileDialog();
};

#endif // SERIALIZATION_H
