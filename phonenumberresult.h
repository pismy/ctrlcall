#ifndef PHONENUMBERRESULT_H
#define PHONENUMBERRESULT_H

#include <QString>

class PhoneNumberResult
{
public:
    enum Error {
        NONE,
        INVALID_NUMBER,
        INCOMPLETE_COUNTRY_CODE,
        INVALID_CHAR,
        INVALID_COUNTRY_CODE
    };

    inline PhoneNumberResult(const Error e, const QString &m) : error(e), errorMessage(m) {}
    inline PhoneNumberResult(const QString &r): error(NONE), result(r) {}

    const Error error;
    const QString errorMessage;
    const QString result;

    static PhoneNumberResult parseToE164(const QString &numberToParse = 0, const QString &defaultCC = 0);
//    static QString isValidCountryCode(const QString &countryCode);
    static PhoneNumberResult parseCountryCode(const QString &iE164Number);

};

#endif // PHONENUMBERRESULT_H
