#include "view.h"

#include <QApplication>
#include "panelcanvas.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PanelCanvas canvas;
    View w(canvas);
    w.show();
    return a.exec();
}
