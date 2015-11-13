#include "windowsutil.h"
#include <sstream>
#include <stdlib.h>
#include <stdio.h>


/**
  * Returns a string property
  */
QString WindowsUtil::x11_getStringProperty (Display *disp, Window win, QString name) {
    Atom prop = XInternAtom(disp, name, False), type;
    int form;
    unsigned long remain, len;
    unsigned char *list;

    if (XGetWindowProperty(disp,win,prop,0,1024,False,AnyPropertyType, &type,&form,&len,&remain,&list) != Success) {
        return NULL;
    }

    return QString::fromUtf8((char*)list);
}

/**
  * Returns the pid for a specific window on a display
***/
int* WindowsUtil::x11_getIntProperty(Display *disp, Window win, QString name) {
    Atom prop = XInternAtom(disp, name, False), type;
    int form;
    unsigned long remain, len;
    unsigned char *list;

    if (XGetWindowProperty(disp,win,prop,0,1024,False,AnyPropertyType, &type,&form,&len,&remain,&list) != Success)
        return NULL;

    return (int*)list;
}

/**
  * Returns the active window on a specific display
***/
Window * WindowsUtil::x11_getActiveWindows (Display *disp, unsigned long *len) {
    Atom prop = XInternAtom(disp,"_NET_ACTIVE_WINDOW",False), type;
    int form;
    unsigned long remain;
    unsigned char *list;

    if (XGetWindowProperty(disp,XDefaultRootWindow(disp),prop,0,1024,False,XA_WINDOW, &type,&form,len,&remain,&list) != Success)
        return NULL;

    return (Window*)list;
}

/**
  * Returns process name from pid (processes output from /proc/<pid>/status)
***/
QString WindowsUtil::x11_getProcessName(long pid)
{
    // construct command string
    QString command = "cat /proc/" + QString("%1").arg(pid) + "/status";
    // capture output in a FILE pointer returned from popen
    FILE * output = popen(command.toStdString().c_str(), "r");
    // initialize a buffer for storing the first line of the output
    char buffer[1024];
    // put the contents of the buffer into a QString
    QString line = QString::fromUtf8(fgets(buffer, sizeof(buffer), output));
    // close the process pipe
    pclose(output);
    // take right substring of line returned to get process name
    return line.right(line.length() - 6).replace("\n", "");
}

WindowsUtil::WinInfo *WindowsUtil::getActiveWindow()
{
    unsigned long len;
    Display *disp = XOpenDisplay(NULL);
    Window *list;
    int* p;

    list = (Window*)x11_getActiveWindows(disp,&len);
    if((int)len > 0)
    {
        // return only the first one...
        for (int i=0;i<(int)len;i++) {
            QString title = x11_getStringProperty(disp,list[i],"WM_NAME");
            p = x11_getIntProperty(disp, list[i], "_NET_WM_PID");
            long p_id = 0;
            QString pName;

            if(p!=NULL)
            {
                p_id = *p; // dereference pointer for obtaining pid
                pName = x11_getProcessName(p_id);
            }

            delete p;
            return new WinInfo(title, NULL, p_id, pName);
        }
    }
    delete list;
    XCloseDisplay (disp);
    return NULL;
}
