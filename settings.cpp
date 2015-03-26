#include "settings.h"

Settings::Settings(QObject *parent) : QObject(parent)
{
  load();
}

Settings::~Settings()
{

}

void Settings::load() {
    qDebug() << "load";
    settings = new QSettings(QDir::homePath()+"/.c2c/config", QSettings::IniFormat);
}
void Settings::save() {
    qDebug() << "save";
    settings->sync();
}

QString Settings::getCountryCode() const
{
    return settings->value("general/country_code", "33").toString();
}

void Settings::setCountryCode(const QString &value)
{
    settings->setValue("general/country_code", value);
}

QString Settings::getCallCommand() const
{
    return settings->value("general/call_cmd", "xdg-open https://monsi.sso.francetelecom.fr/index.asp?target=http%3A%2F%2Fclicvoice.sso.francetelecom.fr%2FClicvoiceV2%2FToolBar.do%3Faction%3Ddefault%26rootService%3DINTERNAL_ANNUARY%26to%3D{NUMBER}").toString();
}

void Settings::setCallCommand(const QString &value)
{
    settings->setValue("general/call_cmd", value);
}

void Settings::setAppIgnored(QString appId, bool ignored) {
    settings->setValue("apps/"+appId+".ignored", ignored);
}

void Settings::setAppName(QString appId, QString name) {
    settings->setValue("apps/"+appId+".name", name);
}

bool Settings::getAppIgnored(QString appId) {
    return settings->value("apps/"+appId+".ignored", "false").toBool();
}

QString Settings::getAppName(QString appId) {
    return settings->value("apps/"+appId+".name").toString();
}
