
QT += core gui widgets

# install
#target.path = $$[QT_INSTALL_EXAMPLES]/qtbase/desktop/systray
#sources.files = $$SOURCES $$HEADERS $$RESOURCES $$FORMS systray.pro
#sources.path = $$[QT_INSTALL_EXAMPLES]/qtbase/desktop/systray
#INSTALLS += target sources

#simulator: warning(This example might not fully work on Simulator platform)

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ctrlcall
TEMPLATE = app

HEADERS       = daemon.h \
    settingsdialog.h \
    settings.h \
    phonenumberresult.h \
    windowinfo.h \
    windowsutil.h

SOURCES       = main.cpp \
    daemon.cpp \
    settingsdialog.cpp \
    settings.cpp \
    phonenumberresult.cpp \
    windowinfo.cpp

RESOURCES     = systray.qrc

FORMS += \
    settingsdialog.ui

macx {
    OBJECTIVE_SOURCES += mac.mm windowsutil_mac.mm
    HEADERS += mac.h
    LIBS += -framework Cocoa
}

unix:!macx {
    HEADERS += linux_x11.h
    SOURCES += linux_x11.cpp windowsutil_x11.cpp
    LIBS += -lX11
#    CONFIG += link_pkgconfig
#    PKGCONFIG += x11
}

win32 {
    HEADERS += win.h
    SOURCES += win.cpp windowsutil_win.cpp
    LIBS += -lpsapi
}

