#include <QApplication>
#include "panels/editor.h"
#include "panels/palette.h"
#include "view.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //Editor editorPanel;
    Palette palettePanel;
    Frame frame;
    //frame.currentFrame = editorPanel;
    View view(palettePanel);
    view.show();
    return a.exec();
}
