#include "settingsdialog.h"
#include "ui_settingsdialog.h"

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);
    Settings settings;
    ui->cmdEdit->setText(settings.getCallCommand());
    ui->ccEdit->setText(settings.getCountryCode());
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::on_buttonBox_accepted()
{
    qDebug() << "OK";
}

void SettingsDialog::on_buttonBox_clicked(QAbstractButton *button)
{
    qDebug() << "clicked: " << button->text();
    switch(ui->buttonBox->standardButton(button)) {
    case QDialogButtonBox::Ok: {
        // save
        qDebug() << "save";
        Settings settings;
        settings.setCallCommand(ui->cmdEdit->text());
        settings.setCountryCode(ui->ccEdit->text());
        settings.save();
        break;
    }
    case QDialogButtonBox::RestoreDefaults: {
        // default
        qDebug() << "restore defaults";
        ui->cmdEdit->setText("xdg-open https://monsi.sso.francetelecom.fr/index.asp?target=http%3A%2F%2Fclicvoice.sso.francetelecom.fr%2FClicvoiceV2%2FToolBar.do%3Faction%3Ddefault%26rootService%3DINTERNAL_ANNUARY%26to%3D{NUMBER}");
        ui->ccEdit->setText("33");
        break;
    }
    }
}
