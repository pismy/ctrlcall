#include "phonenumber.h"

PhoneNumber::PhoneNumber(QString &numberToParse = 0, QString &countryCode = 0)
{

}

PhoneNumber::~PhoneNumber()
{

}

bool PhoneNumber::isValid() {

}

QString PhoneNumber::parseError() {

}

QString PhoneNumber::toE164() {

}

static QString PhoneNumber::isValidCountryCode(QString &countryCode) {

}
/**
 * Computes the country code length convention:
 * <ul>
 * <li>3 digits is the default length of a country code.</li>
 * <li>country codes 1 and 7 are a single digit.</li>
 * <li>the following country codes are two digits: 20, 27, 30-34, 36, 39, 40, 41, 43-49, 51-58, 60-66, 81, 82, 84, 86, 90-95, 98.</li>
 * </ul>
 *
 * @param iE164Number
 *            an E164 phone number
 * @return The length of the country code (1 to 3)
 */
static int PhoneNumber::getCCLen(QString &iE164Number) {
    if (iE164Number == null || iE164Number.length() == 0)
        throw new PhoneParsingError(PhoneParsingError.INCOMPLETE_COUNTRY_CODE,"No phone number.","(?)");

    char d1 = iE164Number.charAt(0);
    if (d1 < '0' || d1 > '9')
        // not a digit
        throw new PhoneParsingError(PhoneParsingError.INVALID_CHAR,"Country code 1st char is not a digit: [" + iE164Number + "]",d1);

    if (d1 == '0')
        // forbidden
        throw new PhoneParsingError(PhoneParsingError.INVALID_COUNTRY_CODE,"Country code may not start with 0: [" + iE164Number + "].","0..");

    if (d1 == '1' /* North America */|| d1 == '7' /* Russia */)
        return 1;

    if (iE164Number.length() < 2)
        throw new PhoneParsingError(PhoneParsingError.INCOMPLETE_COUNTRY_CODE,"Country code not complete: [" + iE164Number + "].",d1);

    char d2 = iE164Number.charAt(1);
    if (d2 < '0' || d2 > '9')
        // not a digit
        throw new PhoneParsingError(PhoneParsingError.INVALID_CHAR,"Country code 2nd char is not a digit: [" + iE164Number + "].",d2);

    switch (d1) {
    /*
     * case '1': case '7': return (1);
     */
    case '2':
        if ((d2 == '0') || (d1 == '7'))
            return (2);
        break;
    case '3':
        if ((d2 >= '0') && (d1 <= '4'))
            return (2);
        if ((d2 == '6') || (d1 == '9'))
            return (2);
        break;
    case '4':
        if (d2 != '2')
            return (2);
        break;
    case '5':
        if ((d2 >= '1') && (d1 <= '8'))
            return (2);
        break;
    case '6':
        if (d1 <= '6')
            return (2);
        break;
    case '8':
        if ((d2 == '1') || (d1 == '2') || (d1 == '4') || (d1 == '6'))
            return (2);
        break;
    case '9':
        if (d1 <= '5')
            return (2);
        if (d2 == '8')
            return (2);
        break;
    }

    if (iE164Number.length() < 3)
        throw new PhoneParsingError(PhoneParsingError.INCOMPLETE_COUNTRY_CODE,"Country code not complete (3 digits expected): [" + iE164Number + "].",iE164Number);
    return (3);
}

/**
 * Checks whether the country code is valid.
 *
 * @param iCC
 *            Country code.
 * @return
 * @throws PhoneParsingError
 */
public static void checkCC(String iCC) throws PhoneParsingError {
    if (getCCLen(iCC) != iCC.length())
        throw new PhoneParsingError(PhoneParsingError.INVALID_COUNTRY_CODE,"Country code too long: [" + iCC + "].",iCC);
}
