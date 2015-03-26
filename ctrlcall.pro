
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
    utils.h \
    phonenumber.h

SOURCES       = main.cpp \
    daemon.cpp \
    settingsdialog.cpp \
    settings.cpp \
    utils.cpp \
    phonenumber.cpp

RESOURCES     = systray.qrc

FORMS += \
    settingsdialog.ui
