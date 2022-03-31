#include <QApplication>
#include "panels/editor.h"
#include "view.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Editor editorPanel;
    View view(editorPanel);
    view.show();
    return a.exec();
}
