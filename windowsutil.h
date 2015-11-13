#ifndef WINDOWSUTIL_H
#define WINDOWSUTIL_H

//#ifdef Q_OS_LINUX
//#include <QtCore>
#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
//#endif

#include <QString>

class WindowsUtil
{
public:
    struct WinInfo {
        WinInfo(QString title, QString appName, long processId, QString processName) {
            title = title;
            appName = appName;
            processId = processId;
            processName = processName;
        }

        QString title;
        QString appName;
        long processId;
        QString processName;
    };

    static WinInfo* getActiveWindow();

#ifdef Q_OS_LINUX
private:
    static QString x11_getStringProperty (Display *disp, Window win, QString name);
    static int* x11_getIntProperty(Display *disp, Window win, QString name);
    static Window * x11_getActiveWindows (Display *disp, unsigned long *len);
    static QString x11_getProcessName(long pid);
#endif

//    WindowsUtil();
};

#endif // WINDOWSUTIL_H
