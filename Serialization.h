#ifndef SERIALIZATION_H
#define SERIALIZATION_H
#include <QString>
#include <QObject>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QFileDialog>
#include <QMessageBox>

class Serialization : public QObject {
    Q_OBJECT
public:
    explicit Serialization(QObject *parent = nullptr);
    // Use an enum so that a typo will give a compile-time error instead of crashing at runtime
    enum Key
    {
        Width,
        Height,
        Frames,
        FramesCount,
        PaletteColumnCount,
        PaletteRowCount,
        ZoomScale
    };
    QJsonValue get(Key);
    void set(Key, QJsonValue);
    int getInt(Key);
private:
    // Would use map, but need iteration to respect insertion order to match up with key enum
    QVector<QPair<QString, QJsonValue>> configDefaults;
    QJsonObject config;
    bool saved;

public slots:
    void SaveAsFile(bool checked);
    void OpenFile(bool checked);
    void NewFile(bool checked);
    void SaveFile(bool checked);
    void loadedSerializedValues(QJsonObject);
signals:
    void openFileExplorer();
    void saveFileDialog(QByteArray);
    void updateViewValue(Serialization::Key, QJsonValue);
    void openNewFile();
};

#endif // SERIALIZATION_H
