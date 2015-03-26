#ifndef PHONENUMBER_H
#define PHONENUMBER_H

class PhoneNumber
{
public:
    explicit PhoneNumber(QString &numberToParse = 0, QString &countryCode = 0);
    ~PhoneNumber();

    bool isValid();
    QString parseError();
    QString toE164();
    static QString isValidCountryCode(QString &countryCode);
};

#endif // PHONENUMBER_H
