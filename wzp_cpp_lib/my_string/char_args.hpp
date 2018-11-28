#ifndef WZP_MY_STRING_CHAR_ARGS_HPP_
#define WZP_MY_STRING_CHAR_ARGS_HPP_

/**
 * Some functions to help handle the chars
 * similar to the function in cctypes
 */

namespace wzp {

/** Convert the char to lower and to upper */
/**
 * convert the upper case char to lower
 * @param  in upper case char
 * @return    lower char
 */
inline static char tolower(char in) {
    if (in <= 'Z' && in >= 'A')
        return in - ('Z' - 'z');
    return in;
}

/**
 * convert the lower case char to uppper
 * @param  in lower case char
 * @return    to uppper case
 */
inline static char touppper(char in) {
    if(in <= 'z' && in >= 'a')
        return in + ('Z' - 'z');
    return in;
}

/*!
 * \brief Inline implementation of isspace(). Tests whether the given character
 *        is a whitespace letter.
 * \param c Character to test
 * \return Result of the test
 */
inline bool isspace(char c) {
  return (c == ' ' || c == '\t' || c == '\r' || c == '\n' || c == '\f');
}

/*!
 * \brief Inline implementation of isblank(). Tests whether the given character
 *        is a space or tab character.
 * \param c Character to test
 * \return Result of the test
 */
inline bool isblank(char c) {
  return (c == ' ' || c == '\t');
}

/*!
 * \brief Inline implementation of isdigit(). Tests whether the given character
 *        is a decimal digit
 * \param c Character to test
 * \return Result of the test
 */
inline bool isdigit(char c) {
  return (c >= '0' && c <= '9');
}

/*!
 * \brief Tests whether the given character is a valid letter in the string
 *        representation of a floating-point value, i.e. decimal digits,
 *        signs (+/-), decimal point (.), or exponent marker (e/E).
 * \param c Character to test
 * \return Result of the test
 */
inline bool isdigitchars(char c) {
  return (c >= '0' && c <= '9')
    || c == '+' || c == '-'
    || c == '.'
    || c == 'e' || c == 'E';
}

} //wzp


#endif /*WZP_MY_STRING_CHAR_ARGS_HPP_*/