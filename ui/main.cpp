#include "notebook.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    notebook window;
    window.show();
    return app.exec();
}
