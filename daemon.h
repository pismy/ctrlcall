#ifndef DAEMON_H
#define DAEMON_H

#include <QObject>
#include <QApplication>
#include <QSystemTrayIcon>
#include <QClipboard>
#include <QDebug>
#include "settings.h"
#include "phonenumberresult.h"

class QSystemTrayIcon;
class QAction;
class QMenu;

class Daemon : public QObject
{
    Q_OBJECT

public:
    explicit Daemon(QObject *parent = 0);
    ~Daemon();

signals:

public slots:
    void onClipboardChanged();
    void toggleState();
    void showSettings();
    void quit();

private:
    Settings *settings;
    bool state;
    bool getState() const;
    void setState(bool value);
    void createSysTray();

    QAction *toggleStateAction;
    QAction *settingsAction;
    QAction *quitAction;

    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;
};

#endif // DAEMON_H
