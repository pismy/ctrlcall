#include "daemon.h"
#include "settingsdialog.h"
#include "phonenumberresult.h"
#include "windowinfo.h"
#ifdef Q_OS_WIN32
#include "win.h"
#endif

#ifdef Q_OS_DARWIN
#include "mac.h"
#endif

#ifdef Q_OS_LINUX
#include "linux_x11.h"
#endif

#include <QAction>
#include <QMenu>

Daemon::Daemon(QObject *parent) : QObject(parent)
{
    // create systray
    createSysTray();

    // listen to clipboard
    setState(true);
}

Daemon::~Daemon()
{

}
void Daemon::toggleState() {
    setState(!getState());
}

void Daemon::setState(bool value) {
    qDebug() << "setState: " << value;
    if(Daemon::state != value) {
        Daemon::state = value;
        QClipboard *clipboard = QApplication::clipboard();
        if(value) {
            connect(clipboard, SIGNAL(dataChanged()), this, SLOT(onClipboardChanged()));
            toggleStateAction->setText("&Pause");
            toggleStateAction->setIcon(QIcon(":/images/pause.png"));
        } else {
            disconnect(clipboard, SIGNAL(dataChanged()), this, SLOT(onClipboardChanged()));
            toggleStateAction->setText("&Resume");
            toggleStateAction->setIcon(QIcon(":/images/play.png"));
        }
    }
}
void Daemon::showSettings()
{
    qDebug() << "showSettings";
    SettingsDialog dlg;
    dlg.exec();
    qDebug() << "settings closed";
}
void Daemon::quit()
{
    qDebug() << "quit";
    QApplication::exit(0);
}
bool Daemon::getState() const
{
    return state;
}

void Daemon::onClipboardChanged()
{
    PhoneNumberResult res = PhoneNumberResult::parseToE164(QApplication::clipboard()->text());
    if(res.error == PhoneNumberResult::NONE) {
        qDebug() << "looks like a phone number: " << res.result;

    QList<WindowInfo> m_activeWindows;

#ifdef Q_OS_WIN32
    win w;
    m_activeWindows = w.getActiveWindows();
#endif

#ifdef Q_OS_DARWIN
    Mac m;
    m_activeWindows = m.getActiveWindows();
#endif

#ifdef Q_OS_LINUX
    linux_x11 l;
    m_activeWindows = l.getActiveWindows();
#endif
    for(int i = 0; i < m_activeWindows.count(); i++)
        qDebug() << "PID: " << m_activeWindows[i].getPID() << " Process Name: " << m_activeWindows[i].getProcessName() << " Window Title: " << m_activeWindows[i].getWindowTitle();

    } else {
        qDebug() << "not a phone number: " << QApplication::clipboard()->text();
    }
}

void Daemon::createSysTray()
{
    // create systray
    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(QIcon(":/images/sipphone.png"));
    trayIcon->setObjectName("CTRL-Phone");
    trayIcon->setToolTip("CTRL-Phone");

    // create ctx menu actions
    toggleStateAction = new QAction(QIcon(":/images/pause.png"), tr("&Pause"), this);
    connect(toggleStateAction, SIGNAL(triggered()), this, SLOT(toggleState()));

    settingsAction = new QAction(QIcon(":/images/settings.png"), tr("&Settings"), this);
    connect(settingsAction, SIGNAL(triggered()), this, SLOT(showSettings()));

    quitAction = new QAction(QIcon(":/images/exit.png"), tr("&Quit"), this);
    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));

    // create menu
    trayIconMenu = new QMenu("CTRL-Phone");
    trayIconMenu->addAction(toggleStateAction);
    trayIconMenu->addAction(settingsAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(quitAction);

    // attach menu and show systray
    trayIcon->setContextMenu(trayIconMenu);
    trayIcon->setVisible(true);
    trayIcon->show();
}
