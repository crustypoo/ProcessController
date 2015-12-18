/*
 * err_handles.h
 * A collection of all error messaging functions to be utilized by the subsystems of longcat.
 *  Created on: Sep 17, 2015
 *      Author: oscott
 */

#ifndef ERR_HANDLES_H
#define ERR_HANDLES_H

#include <mysql.h>
#include <errno.h>

void print_sys_err_msg(char * const file, const int errnum, char * const func, char * const msg);

void print_prog_err_msg(char * const file, char * const func, char * const msg);

void print_prog_warn_msg(char * const file, char * const func, char * const msg);

void print_db_err(char * const function, MYSQL * const mariadb);

void print_stmt_err(char * const function, MYSQL_STMT * const stmt);

void print_param_err(const char * const func, const char * const err_msg);

/*
 * Keyence specific error messages
 * */
void print_sys_err(const char * const err_type, const char * const err_msg);

void print_err(const char * const func, const char * const err_msg);

void print_key_err_msg(const char * const err_type, const char * const err_msg);

void print_key_format_err(const char * const err_msg);

#endif
