#include "utils.h"
#include <X11/Xlib.h>
#include <X11/Xatom.h>

Utils::Utils()
{

}

Utils::~Utils()
{

}

//static int Utils::getActiveWindowId() {
//    Display *display;
//    Window active;
//    int revert;

//    display = XOpenDisplay(NULL);
//    XGetInputFocus(display, &active, &revert);

//    qDebug() << "active window: " << active;

//    Atom winName = XInternAtom(display, "WM_NAME", True);
//    Atom netWinName = XInternAtom(display, "_NET_WM_NAME", True);
//    Atom winClass = XInternAtom(display, "WM_CLASS", True);

//    XGetWindowProperty()
//}

//static QString Utils::run() {
//    XGetWindowProperty(Display)
//    XGetInputFocus()
//            QX11I
//}
//void check_status(int status, unsigned long window)
//{
//    if (status == BadWindow) {
//        printf("window id # 0x%lx does not exists!", window);
//        exit(1);
//    }

//    if (status != Success) {
//        printf("XGetWindowProperty failed!");
//        exit(2);
//    }
//}

//unsigned char* get_string_property(char* property_name)
//{
//    Atom actual_type, filter_atom;
//    int actual_format, status;
//    unsigned long nitems, bytes_after;

//    filter_atom = XInternAtom(display, property_name, True);
//    status = XGetWindowProperty(display, window, filter_atom, 0, MAXSTR, False, AnyPropertyType,
//                                &actual_type, &actual_format, &nitems, &bytes_after, &prop);
//    check_status(status, window);
//    return prop;
//}

//unsigned long get_long_property(char* property_name)
//{
//    get_string_property(property_name);
//    unsigned long long_property = prop[0] + (prop[1]<<8) + (prop[2]<<16) + (prop[3]<<24);
//    return long_property;
//}

//int main(int argc, char** argv)
//{
//    char *display_name = NULL;  // could be the value of $DISPLAY

//    display = XOpenDisplay(display_name);
//    if (display == NULL) {
//        fprintf (stderr, "%s:  unable to open display '%s'\n", argv[0], XDisplayName (display_name));
//    }
//    int screen = XDefaultScreen(display);
//    window = RootWindow(display, screen);

//    window = get_long_property("_NET_ACTIVE_WINDOW");

//    printf("_NET_WM_PID: %lu\n", get_long_property("_NET_WM_PID"));
//    printf("WM_CLASS: %s\n", get_string_property("WM_CLASS"));
//    printf("_NET_WM_NAME: %s\n", get_string_property("_NET_WM_NAME"));

//    XCloseDisplay(display);
//    return 0;
//}
