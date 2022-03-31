#ifndef PALETTE_H
#define PALETTE_H

#include <QObject>
#include <QSize>
#include <QVector>

class Palette : public QObject
{
    Q_OBJECT
public:
    explicit Palette(QObject *parent = nullptr);
private:
    QSize paletteSize;
    // TODO: maybe put this into a .hex file in the res folder and read from there
    QVector<QString> defaultPalette;

public slots:
    void paletteColumnsChanged(int);
    void paletteRowsChanged(int);
signals:
    void updateViewPalette(const QVector<QString>&, QSize);
};

#endif // PALETTE_H
