#include <QDebug>
#include "panels/palette.h"
// Authors
// Devin White
// Xuyen Nguyen
// Taylor Adamson
// Ansam Al Sharif
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
        "333c57",
        "577A3A",
        "8B4789",
        "CD7054",
        "CDAD00",
        "D0FAEE",
        "FCB514",
        "33FF33",
        "EAB5C5",
        "3E7A5E"
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
