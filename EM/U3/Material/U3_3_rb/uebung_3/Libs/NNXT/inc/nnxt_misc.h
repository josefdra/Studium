/**
 * @file
 * nnxt_misc.h
 *
 * @todo rework all timing functions, e.g. take care of overflows and add automatic conversations if the systick changes (or something like this...)
 */

#ifndef NXT_APPLICATIONDRIVER_INC_NNXT_MISC_H_
#define NXT_APPLICATIONDRIVER_INC_NNXT_MISC_H_
/*********************************/
/*           includes            */

/*********************************/
/*           macros              */

/*********************************/
/*      extern variables         */


/*********************************/
/*          enums                */

/*********************************/
/*          structs              */

/*********************************/
/*        typedefs               */

/*********************************/
/*      Function prototypes      */

/**
 * @brief      Initiates a software system reset.
 */
void SystemSoftwareReset(void);

/**
 * @brief      Initiates a software stop.
 */
void SoftwareStop(void);

/**
 * @brief      sprint for doubles
 */
void sprintDouble(char *s, double v, int decimalDigits);

#endif /* NXT_APPLICATIONDRIVER_INC_NNXT_MISC_H_ */
