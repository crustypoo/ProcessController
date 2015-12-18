/*
 * err_handles.c
 *
 *  Created on: Sep 17, 2015
 *      Author: oscott
 */

#include <stdlib.h>		/* printf */
#include <string.h>     /* strerror function */
#include <stdio.h> 		/* prtinf function 	*/
#include "err_handles.h"
/*
 * prints an error message as the result of a failed system call.
 * NOTE: This means errno will be set within caller program.
 *
 * inputs:  file  - The file in which the error occurred.
 * 			errno - The error value passed from the caller
 * 			func  - The caller function
 * 			msg   - The message to be printed
 */
void print_sys_err_msg(char * const file, const int errnum, char * const func, char * const msg) {
	int i = printf("\nERROR: ERR#%i: %s\n   from %s @ %s - %s.", errnum,
			strerror(errnum), file, func, msg);

	fflush(stdout);
}

/*
 * Prints a program error message. This is a unique message specified by the caller
 * program.
 *
 * inputs:  file - The file in which the error occurred.
 * 			func - The caller function
 * 			msg  - The message to be printed
 */
void print_prog_err_msg(char * const file, char * const func, char * const msg) {
	printf("\nERROR: %s @ %s- %s.", file, func, msg);
	fflush(stdout);
}

/*
 * Prints a program warning message. This is a unique message specified by the caller
 * program.
 *
 * inputs:  file - The file in which the error occurred.
 * 			func - The caller function
 * 			msg  - The message to be printed
 */
void print_prog_warn_msg(char * const file, char * const func, char * const msg) {
	printf("\nWARNING: %s - %s.", func, msg);
	fflush(stdout);
}

/*
 * Prints mysql error due to db issues
 *
 * mysql: The mysql connection reference that caused the error.
 */
void print_db_err(char * const function, MYSQL * const mariadb) {
	printf("\nError(%d) [%s] \"%s\"", mysql_errno(mariadb),
			mysql_sqlstate(mariadb), mysql_error(mariadb));
	fflush(stdout);

	mysql_close(mariadb);
}

/*
 * Prints mysql error due to predefined statement issues
 *
 * stmt: The mysql predefined statement reference that caused the error.
 */
void print_stmt_err(char * const function, MYSQL_STMT * const stmt) {
	printf("\nError(%i) [%s] \"%s\"", mysql_stmt_errno(stmt),
			mysql_stmt_sqlstate(stmt), mysql_stmt_error(stmt));
	fflush(stdout);
	mysql_stmt_close(stmt);
}

/*
 * Prints error due to paraemter issues
 *
 * Parameters:
 * func:    The function calling print_param_err
 * err_msg: Reason for error
 */
void print_param_err(const char * const func, const char * const err_msg) {
	printf("\nError, %s: %s", func, err_msg);
	fflush(stdout);
}

/*
 * Prints unique error message.
 *
 * inputs- func:    The calling function
 *         err_msg: Cause of error
 */
void print_err(const char * const func, const char * const err_msg) {
	printf("\nERROR: %s- %s.", func, err_msg);
	fflush(stdout);
}

/**
 * Prints error message if a read/write fflushsystem call error occurs.
 *
 * Parameters:
 */
void print_sys_err(const char * const err_type, const char * const err_msg) {
	printf("\n%s %s %s      System Message: %s\n", "SYSTEM CALL", err_type,
			err_msg, strerror(errno));
	fflush(stdout);
}

/**
 * Prints error code received by keyence. Error messages are of the format
 * "ER,__,XX<CR>"
 */
void print_key_err_msg(const char * const err_type, const char * const err_msg) {
	printf("\nKeyence %s Error Message: ER,%c%c,%c%c", err_type, err_msg[3],
			err_msg[4], err_msg[6], err_msg[7]);
	fflush(stdout);
}

/**
 * Prints error message if out of format data is found.
 */
void print_key_format_err(const char * const err_msg) {
	printf("\nFORMAT ERROR: Keyence return data incorrectly formatted. \n         ---%s",
			err_msg);
	fflush(stdout);
}
