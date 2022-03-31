#include <QDebug>
#include "panels/palette.h"

Palette::Palette(QObject* parent) : QObject(parent)
{
    defaultPalette = {
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
}

void Palette::paletteColumnsChanged(int columns)
{
    paletteSize.setWidth(columns);
    emit updateViewPalette(defaultPalette, paletteSize);
}

void Palette::paletteRowsChanged(int rows)
{
    paletteSize.setHeight(rows);
    emit updateViewPalette(defaultPalette, paletteSize);
}
