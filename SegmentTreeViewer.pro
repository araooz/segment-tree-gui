QT += core gui widgets

CONFIG += c++17

TARGET = SegmentTreeViewer
TEMPLATE = app

SOURCES += \
    main.cpp

HEADERS += \
    segment_tree.h \
    treewidget.h \
    mainwindow.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target