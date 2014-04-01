TEMPLATE = app
TARGET   = framework
DESTDIR  = bin
CONFIG  += debug_and_release build_all

HEADERS = escenadrawer.h \
    escena.h \
    preview.h \
    previewdrawer.h \
    textura.h \
    render.h
SOURCES = main.cpp \
    escenadrawer.cpp \
    escena.cpp \
    preview.cpp \
    previewdrawer.cpp \
    textura.cpp \
    render.cpp


QT += opengl

# For windows
win32 {
    INCLUDEPATH += "./" \
        "C:\Windows\Cg\include"

    LIBS += -L"C:\Windows\Cg\lib" \
        -lcg \
        -lcgGL \
        -lglut32 \
        -lGLU
}
# For Linux
else{
    INCLUDEPATH += "./cg-include/"

    LIBS += -L"./cg-lib/" \
            -lCg \
            -lCgGL \
            -lglut \
            -lGLU
}


CONFIG(debug, debug|release) {
    # Only for debug
    CONFIG     += console
    OBJECTS_DIR = Debug

    build_pass:TARGET = $$join(TARGET,,,_d)

} else {
    # Only for release
    OBJECTS_DIR = Release
    QMAKE_CXXFLAGS_RELEASE += -DNDEBUG
}


MOC_DIR = $$OBJECTS_DIR
UI_DIR  = $$OBJECTS_DIR
RCC_DIR = $$OBJECTS_DIR

OTHER_FILES += \
    vertexE.cg \
    vertex.cg
