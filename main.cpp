#include <QApplication>
#include "panels/editor.h"
#include "view.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Editor editorPanel;
    View w(editorPanel);
    w.show();
    return a.exec();
}
