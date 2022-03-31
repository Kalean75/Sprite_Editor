#ifndef PALETTE_H
#define PALETTE_H

#include <QObject>
#include <QSize>

class Palette : public QObject
{
    Q_OBJECT
public:
    explicit Palette(QObject *parent = nullptr);
private:
    QSize paletteSize;
    // TODO: maybe put this into a .hex file in the res folder and read from there
    QVector<QString> defaultPalette
    {
        "1a1c2c",
        "5d275d",
        "b13e53",
        "ef7d57",
        "ffcd75",
        "a7f070",
        "38b764",
        "257179",
        "29366f",
        "3b5dc9",
        "41a6f6",
        "73eff7",
        "f4f4f4",
        "94b0c2",
        "566c86",
        "333c57"
    };
public slots:
    void paletteColumnsChanged(int);
    void paletteRowsChanged(int);
signals:
    void updateViewPalette(const QVector<QString>&, QSize);
};

#endif // PALETTE_H
