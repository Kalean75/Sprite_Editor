#include <QApplication>
#include "panels/editor.h"
#include "panels/palette.h"
#include "Serialization.h"
#include "view.h"
// Authors
// Devin White
// Xuyen Nguyen
// Taylor Adamson
// Ansam Al Sharif
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //Editor editorPanel;
    Palette palettePanel;
    Frame frame;
    Serialization serialization;
    //frame.currentFrame = editorPanel;
    View view(palettePanel, serialization);
    view.show();
    return a.exec();
}
