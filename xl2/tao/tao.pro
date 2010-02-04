# #####################################################################
# Automatically generated by qmake (2.01a) Fri May 12 17:15:46 2006
# #####################################################################
TEMPLATE = app
TARGET = 
DEPENDPATH += . \
    ../xlr
INCLUDEPATH += . \
    ../xlr
QT += opengl \
    svg

# Tell the XLR portion that we are building for Tao
DEFINES += TAO CONFIG_MACOSX

# Input
HEADERS += glwidget.h
SOURCES += glwidget.cpp \
    tao_main.cpp \
    ../xlr/tree.cpp \
    ../xlr/syntax.cpp \
    ../xlr/scanner.cpp \
    ../xlr/runtime.cpp \
    ../xlr/renderer.cpp \
    ../xlr/parser.cpp \
    ../xlr/options.cpp \
    ../xlr/opcodes.cpp \
    ../xlr/main.cpp \
    ../xlr/errors.cpp \
    ../xlr/context.cpp \
    ../xlr/compiler.cpp \
    ../xlr/basics.cpp

RESOURCES += framebufferobject.qrc

LIBS += -L../xlr \
    -lxlr

# LLVM dependencies
LLVM_PATH = /usr/local/bin
LLVM_FLAGS = $$system($$LLVM_PATH/llvm-config --cppflags | sed -e s/-DNDEBUG//g)
LLVM_LIBS = $$system($$LLVM_PATH/llvm-config --ldflags --libs core jit native)
LLVM_INC = $$system($$LLVM_PATH/llvm-config --includedir)
LLVM_DEF = $$system($$LLVM_PATH/llvm-config --cppflags | grep -o .D_.* | sed s/-D//g)
DEFAULT_FONT = /Library/Fonts/Arial.ttf
LIBS += $$LLVM_LIBS
DEFINES += $$LLVM_DEF
