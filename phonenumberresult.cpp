#include "phonenumberresult.h"
#include "qchar.h"

/*
 * 0123456789
 *
 */
PhoneNumberResult PhoneNumberResult::parseToE164(const QString &numberToParse/*, const QString &countryCode = 0*/) {
    QString e164;
    for(int i=0; i<numberToParse.length(); i++) {
        switch (numberToParse[i].toLatin1()) {
        case '.':
        case '-':
        case '(':
        case ')':
        case ' ':
            // ignore;
            break;
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            e164+=numberToParse[i];
            break;
        default:
            return PhoneNumberResult(PhoneNumberResult::INVALID_CHAR, "Invalid char found at index "+QString::number(i)+": ["+numberToParse+"]");
        }
    }
    return PhoneNumberResult(e164);
}

PhoneNumberResult PhoneNumberResult::parseCountryCode(const QString &iE164Number) {
    if (iE164Number == NULL || iE164Number.length() == 0) {
        //        throw new PhoneParsingError(PhoneParsingError.INCOMPLETE_COUNTRY_CODE,"No phone number.","(?)");
        return PhoneNumberResult(PhoneNumberResult::INCOMPLETE_COUNTRY_CODE, "No phone number.");
    }

    QChar d1 = iE164Number[0];
    if (d1 < '0' || d1 > '9')
        // not a digit
        return PhoneNumberResult(PhoneNumberResult::INVALID_CHAR,"Country code 1st char is not a digit: [" + iE164Number + "]");

    if (d1 == '0')
        // forbidden
        return PhoneNumberResult(PhoneNumberResult::INVALID_COUNTRY_CODE,"Country code may not start with 0: [" + iE164Number + "].");

    if (d1 == '1' /* North America */|| d1 == '7' /* Russia */)
        return PhoneNumberResult(d1);

    if (iE164Number.length() < 2)
        return PhoneNumberResult(PhoneNumberResult::INCOMPLETE_COUNTRY_CODE,"Country code not complete: [" + iE164Number + "].");

    QChar d2 = iE164Number[1];
    if (d2 < '0' || d2 > '9')
        // not a digit
        return PhoneNumberResult(PhoneNumberResult::INVALID_CHAR,"Country code 2nd QChar is not a digit: [" + iE164Number + "].");

    switch (d1.toLatin1()) {
    /*
     * case '1': case '7': return (1);
     */
    case '2':
        if ((d2 == '0') || (d2 == '7'))
            return PhoneNumberResult(iE164Number.left(2));
        break;
    case '3':
        if ((d2 >= '0') && (d2 <= '4'))
            return PhoneNumberResult(iE164Number.left(2));
        if ((d2 == '6') || (d2 == '9'))
            return PhoneNumberResult(iE164Number.left(2));
        break;
    case '4':
        if (d2 != '2')
            return PhoneNumberResult(iE164Number.left(2));
        break;
    case '5':
        if ((d2 >= '1') && (d2 <= '8'))
            return PhoneNumberResult(iE164Number.left(2));
        break;
    case '6':
        if (d2 <= '6')
            return PhoneNumberResult(iE164Number.left(2));
        break;
    case '8':
        if ((d2 == '1') || (d2 == '2') || (d2 == '4') || (d2 == '6'))
            return PhoneNumberResult(iE164Number.left(2));
        break;
    case '9':
        if (d2 <= '5')
            return PhoneNumberResult(iE164Number.left(2));
        if (d2 == '8')
            return PhoneNumberResult(iE164Number.left(2));
        break;
    }

    if (iE164Number.length() < 3)
        return PhoneNumberResult(PhoneNumberResult::INCOMPLETE_COUNTRY_CODE,"Country code not complete (3 digits expected): [" + iE164Number + "].");
    return PhoneNumberResult(iE164Number.left(3));
}
