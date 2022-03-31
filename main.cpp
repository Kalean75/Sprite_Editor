#include <QApplication>
#include "panels/editor.h"
#include "panels/palette.h"
#include "view.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Editor editorPanel;
    Palette palettePanel;
    View view(editorPanel, palettePanel);
    view.show();
    return a.exec();
}
