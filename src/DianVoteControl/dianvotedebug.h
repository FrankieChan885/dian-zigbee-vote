#ifndef DIANVOTEDEBUG_H
#define DIANVOTEDEBUG_H

//#define DO_ROLL_CALL                    // whether do roll call

/*
 * This macro is defined to decide whether to allow
 * a hand-held terminal to send data several times,
 * for testing
 */
#define TEST_RECEIVE_DATA_CONTINUE

/*
 * This macro is defined in the voting process to
 * decide whether the results are displayed
 */
#define DISPLAY_RESULT_DURING_VOTING

/*
 * This macro determines whether all the messages:
 * errors, warnings, fatal errors and debug messages
 * redirected to a file
 */
#define REDIRECT_MESSAGE_TO_FILE

#endif // DIANVOTEDEBUG_H
