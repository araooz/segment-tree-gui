#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    MainWindow window;
    window.show();

    return app.exec();
}
/* Correr con:

qmake SegmentTreeViewer.pro
make
./SegmentTreeViewer

*/