#include <QApplication>
#include <QMessageBox>

//#include "window.h"
#include "daemon.h"
#include "settingsdialog.h"

int main(int argc, char *argv[])
{
     Q_INIT_RESOURCE(systray);

     QApplication app(argc, argv);

     if (!QSystemTrayIcon::isSystemTrayAvailable()) {
         QMessageBox::critical(0, QObject::tr("Systray"),
                               QObject::tr("I couldn't detect any system tray "
                                           "on this system."));
         return 1;
     }
     QApplication::setQuitOnLastWindowClosed(false);

//     Window window;
//     window.show();

    Daemon daemon;

//    SettingsDialog dlg;
//    dlg.show();

     return app.exec();
}
