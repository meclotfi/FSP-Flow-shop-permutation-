QT += widgets
requires(qtConfig(combobox))
qtHaveModule(printsupport): QT += printsupport

FORMS     = mainwindowbase.ui
HEADERS   = mainwindow.h \
    testmodel.h
SOURCES   = main.cpp \
            mainwindow.cpp \
            testmodel.cpp

# install
target.path = $$[QT_INSTALL_EXAMPLES]/widgets/painting/fontsampler
INSTALLS += target
QMAKE_CXXFLAGS += -fopenmp
LIBS += -fopenmp
