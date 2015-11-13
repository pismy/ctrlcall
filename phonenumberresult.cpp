#include "phonenumberresult.h"
#include "qchar.h"

/*
 * 0123456789
 * 06 30 07 29 35
 * +33 6 30 07 29 35
 * 33630072935
 * 0033630072935
 */
PhoneNumberResult PhoneNumberResult::parseToE164(const QString &numberToParse, const QString &defaultCC) {
    /*
     * Algorithme du parsing:
     *  ----------------------
     * On tente de dégager un
     * premier groupe de chiffres du reste des chiffres: - ce groupe peut
     * être entre parenthèses, - ou bien séparé du reste (espace, - ou .) -
     * ce group peut être préfixé d'un '+' - tout autre caractère est
     * interdit
     *
     * si(on est arrivé à dégager un premier groupe de chiffres) { si(un + a
     * été lu ou que ce groupe était entre parenthèses) { alors il s'agit
     * d'un numéro international et le premier groupe est le code pays }
     * sinon si(le groupe commence par 0) { on considère que c'est un numéro
     * national, et on utilise le code pays par défaut } sinon // le groupe
     * ne commence pas par 0 { on considère que c'est un numéro
     * international et le premier groupe est le code pays }
     *
     * On finit de lire le reste du numéro - on lit tous les chiffres, - on
     * ignore les séparateurs (blanc, - ou .) - tout autre caractère est
     * interdit } sinon // on ne peut dégager un premier groupe { si(le
     * groupe commence par 0) { on considère que c'est un numéro national,
     * et on utilise le code pays par défaut } sinon // le groupe ne
     * commence pas par 0 { c'est un numéro au format E.164: on tente de
     * séparer le code pays du numéro. } }
     *
     * On valide le code pays, ensuite on valide le numéro en fonction du
     * pays (nombre de chiffres?), ensuite on valide éventuellement que
     * c'est un numéro de mobile.
     */
    if (numberToParse == NULL || numberToParse.isEmpty()) {
        return PhoneNumberResult(PhoneNumberResult::INVALID_NUMBER, "No phone number.");
    }

    int i = 0;
    // --- 1: read international country code
    QString countryCode = NULL;
    QString number = NULL;
    if (numberToParse[i] == '(') {
        // --- international format. country code between braces:
        /*
         * ( + <code> ) <number> or ( <code> ) <number>
         */
        // consume '('
        i++;
        // skip blanks
        while (i < numberToParse.length() && numberToParse[i] == ' ') {
            i++;
        }
        // --- accept up to two '+' (optional)
        if (i < numberToParse.length() && numberToParse[i] == '+') {
            i++;
            if (i < numberToParse.length() && numberToParse[i] == '+') {
                i++;
            }
        }

        // --- read digits untill ')'
        QString buffer;
        for (; i < numberToParse.length(); i++) {
            if (numberToParse[i] == ')') {
                // end of country code
                break;
            } else if (numberToParse[i] >= '0' && numberToParse[i] <= '9') {
                // append digit to country code
                buffer.append(numberToParse[i]);
            } else if (numberToParse[i] == ' ') {
                // ignore blanks
                continue;
            } else {
                // invalid char
                return PhoneNumberResult(PhoneNumberResult::INVALID_CHAR, "Unexpected char: " + QString(numberToParse[i]));
            }
        }
        // treat eof
        if (i >= numberToParse.length()) {
            return PhoneNumberResult(PhoneNumberResult::INCOMPLETE_COUNTRY_CODE, "Incomplete: ')' expected.");
        }
        // --- consume ')'
        i++;
        countryCode = buffer;

        // --- read the end of the number
        buffer.clear();
        for (; i < numberToParse.length(); i++) {
            if (numberToParse[i] == ' ' || numberToParse[i] == '-' || numberToParse[i] == '.') {
                // ignore separators
                continue;
            } else if (numberToParse[i] >= '0' && numberToParse[i] <= '9') {
                // append
                buffer.append(numberToParse[i]);
            } else {
                // invalid char
                return PhoneNumberResult(PhoneNumberResult::INVALID_CHAR, "Unexpected char: " + QString(numberToParse[i]));
            }
        }
        number = buffer;
    } else if (numberToParse[i] == '0') {
        // --- national format
        // --- consume '0'
        i++;
        // --- country code is default
        countryCode = defaultCC;

        // --- read the end of the number
        QString buffer;
        for (; i < numberToParse.length(); i++) {
            if (numberToParse[i] == ' ' || numberToParse[i] == '-' || numberToParse[i] == '.')
                // ignore separators
                continue;
            else if (numberToParse[i] >= '0' && numberToParse[i] <= '9')
                // append
                buffer.append(numberToParse[i]);
            else
                // invalid char
                return PhoneNumberResult(PhoneNumberResult::INVALID_CHAR, "Unexpected char: " + QString(numberToParse[i]));
        }
        number = buffer;
    } else {
        // --- international format. country code expected
        /*
         * + <code> {' ', '.', '-'} <number>
         */
        // --- accept up to two '+' (optional)
        if (i < numberToParse.length() && numberToParse[i] == '+') {
            i++;
            if (i < numberToParse.length() && numberToParse[i] == '+') {
                i++;
            }
        }
        // skip blanks
        while (i < numberToParse.length() && numberToParse[i] == ' ') {
            i++;
        }

        // assume first block of digits is the country code
        QString buffer;
        for (; i < numberToParse.length(); i++) {
            if (numberToParse[i] == ' ' || numberToParse[i] == '-' || numberToParse[i] == '.') {
                // --- end of country code
                break;
            } else if (numberToParse[i] >= '0' && numberToParse[i] <= '9') {
                // --- append
                buffer.append(numberToParse[i]);
            } else {
                // --- invalid char
                return PhoneNumberResult(PhoneNumberResult::INVALID_CHAR, "Unexpected char: " + QString(numberToParse[i]));
            }
        }
        if (i >= numberToParse.length()) {
            // --- no more char: that might have been a E.164 format
            // try to split
            QString e164Phone = buffer;
            PhoneNumberResult res = parseCountryCode(countryCode);
            if(res.error != PhoneNumberResult::NONE) {
                return res;
            }

            int ccLen = res.result.length();
            number = e164Phone.right(ccLen);
//            countryCode = e164Phone.substring(0, ccLen);
            countryCode = res.result;
        } else {
            // --- consume separator
            i++;
            countryCode = buffer;

            buffer.clear();

            // --- read the end of the number
            for (; i < numberToParse.length(); i++) {
                if (numberToParse[i] == ' ' || numberToParse[i] == '-' || numberToParse[i] == '.') {
                    // ignore separators
                    continue;
                } else if (numberToParse[i] >= '0' && numberToParse[i] <= '9') {
                    // append
                    buffer.append(numberToParse[i]);
                } else {
                    // invalid char
                    return PhoneNumberResult(PhoneNumberResult::INVALID_CHAR, "Unexpected char: " + QString(numberToParse[i]));
                }
            }

            number = buffer;
        }
    }

    // --- vérification du code pays (lève une exception si invalide)
    PhoneNumberResult res = parseCountryCode(countryCode);
    if(res.error != PhoneNumberResult::NONE) {
        return res;
    }
    if(res.result.length() != countryCode.length()) {
        return PhoneNumberResult(PhoneNumberResult::INVALID_COUNTRY_CODE, "Invalid country code: "+countryCode);
    }

    // --- le numéro doit avoir au moins 3 chiffres
    if (number.length() < 3) {
        return PhoneNumberResult(PhoneNumberResult::INVALID_NUMBER, "Number should have at least 4 digits.");
    }
    // --- code pays + numéro doit avoir au max 15 chiffres
    if (countryCode.length() + number.length() > 15) {
        return PhoneNumberResult(PhoneNumberResult::INVALID_NUMBER, "Complete number should'nt have more than 15 digits.");
    }
    // --- validation du numéro en fonction du pays
    int ccInt = countryCode.toInt();
    switch (ccInt) {
    case 33: // France
        // --- le numéro ne peut commencer par '0'
        if (number[0] == '0') {
            return PhoneNumberResult(PhoneNumberResult::INVALID_NUMBER, "French numbers should not start with '0'.");
        }
        break;
    }

    // return "(+" + countryCode + ")" + (number.toString());
    // return in E.164 format
    return PhoneNumberResult(countryCode + number);
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
