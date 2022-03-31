#include <QDebug>
#include "panels/palette.h"

Palette::Palette(QObject* parent) : QObject(parent)
{
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
