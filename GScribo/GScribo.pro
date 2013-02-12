# -------------------------------------------------
# Project created by QtCreator 2013-01-21T09:20:54
# -------------------------------------------------
QT += xml
INCLUDEPATH += /lrde/home/stage/froger_a/olena/scribo/ \
    /lrde/home/stage/froger_a/olena/_build/scribo/demo/
QMAKE_CXXFLAGS += -DNDEBUG \
    -DMLN_WO_GLOBAL_VARS
LIBS += -I/usr/include/graphicsImage \
    -lGraphicsMagick++ \
    -ltesseract_full
TARGET = GScribo
TEMPLATE = app
SOURCES += main.cpp \
    mainwindow.cpp \
    xml.cpp \
    Rendering/scene.cpp \
    Rendering/selection.cpp \
    Rendering/regionitem.cpp \
    PagesWidget/pageswidget.cpp \
    PagesWidget/pagesmodel.cpp \
    PagesWidget/pagesdelegate.cpp \
    Preferences/segmentationoptions.cpp \
    Preferences/preprocessingoptions.cpp \
    Preferences/ocroptions.cpp \
    Preferences/generaloptions.cpp \
    Preferences/preferencesdialog.cpp \
    Processing/progressdialog.cpp \
    XmlWidget/xmlwidget.cpp \
    XmlWidget/xmlmodel.cpp \
    XmlWidget/xmlitem.cpp \
    XmlWidget/xmlattributes.cpp \
    XmlWidget/attributesmodel.cpp \
    XmlWidget/xmlview.cpp \
    regionwidget.cpp
HEADERS += mainwindow.h \
    variantpointer.h \
    configs.h \
    region.h \
    dir.h \
    xml.h \
    Rendering/scene.h \
    Rendering/selection.h \
    Rendering/regionitem.h \
    PagesWidget/pageswidget.h \
    PagesWidget/pagesmodel.h \
    Preferences/segmentationoptions.h \
    Preferences/preprocessingoptions.h \
    Preferences/ocroptions.h \
    Preferences/generaloptions.h \
    Preferences/optionwidget.h \
    Preferences/preferencesdialog.h \
    Processing/progressdialog.h \
    XmlWidget/xmlwidget.h \
    XmlWidget/xmlmodel.h \
    XmlWidget/xmlitem.h \
    XmlWidget/xmlattributes.h \
    XmlWidget/selectionproxy.h \
    XmlWidget/xmldelegate.h \
    XmlWidget/attributesmodel.h \
    PagesWidget/pagesdelegate.h \
    Rendering/rootgraphicsitem.h \
    XmlWidget/xmlview.h \
    regionwidget.h
FORMS += mainwindow.ui \
    Preferences/preferencesdialog.ui \
    Preferences/ocroptions.ui \
    Preferences/segmentationoptions.ui \
    Preferences/generaloptions.ui \
    Preferences/preprocessingoptions.ui
