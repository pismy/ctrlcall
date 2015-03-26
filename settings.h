#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QDebug>
#include <QSettings>
#include <QDir>

class Settings : public QObject
{
    Q_OBJECT
public:
    explicit Settings(QObject *parent = 0);
    ~Settings();

//    static const QString DEFAULT_CALL_COMMAND("xdg-open https://monsi.sso.francetelecom.fr/index.asp?target=http%3A%2F%2Fclicvoice.sso.francetelecom.fr%2FClicvoiceV2%2FToolBar.do%3Faction%3Ddefault%26rootService%3DINTERNAL_ANNUARY%26to%3D{NUMBER}");
//    static const QString DEFAULT_COUNTRY_CODE("33");

    QString getCallCommand() const;
    void setCallCommand(const QString &value);

    QString getCountryCode() const;
    void setCountryCode(const QString &value);

    void setAppIgnored(QString appId, bool ignored);
    void setAppName(QString appId, QString name);
    bool getAppIgnored(QString appId);
    QString getAppName(QString appId);
    void load();
    void save();

signals:

public slots:

private:
    QSettings *settings;
};

#endif // SETTINGS_H
