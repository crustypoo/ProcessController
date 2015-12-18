/*
 * eurotherm_modbus_subsystem.c
 *
 *  Created on: Dec 14, 2015
 *      Author: oscott
 */

#include "eurotherm_modbus_subsystem.h"
#include "err_handles.h"
#include <modbus.h>
#include <dirent.h> 		/* needed for memset function */
#include <stdlib.h>         /* NULL */
#include <stdio.h>			/* printf, sprintf, memset functions */
#include <string.h>

#define PROG_FILE_NAME "eurotherm_modbus_subsystem.c"

/* Limits for set point values within the Eurotherm */
#define OUT_POW_MIN 0
#define OUT_POW_MAX 100
#define OUT_POW_MIN 0
#define OUT_POW_MIN 0
#define SP_MAX 5000

/* Eurotherm resolution for register addresses*/
#define EURO_SP_RES 1
#define EURO_OUT_POW_RES 0

/* Port config values */
#define MOD_TO 50000
#define BAUD_R 19200
#define DIR_ADDR "/dev/"
#define DEVICE "tty"
#define BROADCAST_ID 0	/* No reply during broadcast communications */

static int update_euro_loop_mode(const modbus_t * const mb_context,
		const loop_num loop, const loop_cntrl_mode mode);

static int write_to_euro_register(const modbus_t * const mb_context,
		const loop_num loop, const reg_type reg, const int reg_value);

/* TODO Sniffs the comm ports and searches for a single comm port. Attempts to load modbus slaves with the addresses
 * ranging from start_id to end_id.
 * */
int automated_eurotherms_connection(const serial_port_type port_type,
		euro_mb_network * const mb_network, const int start_id,
		const int end_id) {
}

/* Updates 'target op' for the given loop, changing the 'loop_cntrl_mode' to 'man_loop'
 * NOTE: The 'Working Output Power' is read only, therefore this is how the control value must be updated.
 * */
int eurotherm_set_op(const modbus_t * const mb_context, const loop_num loop,
		const double out_pow) {
	if ((out_pow < OUT_POW_MIN) || (out_pow > OUT_POW_MAX)) {
		print_prog_err_msg(PROG_FILE_NAME, "eurotherm_set_op",
				"Output power received exceeds allowed range");
		return -1;
	}
	if (!mb_context) {
		print_prog_err_msg(PROG_FILE_NAME, "eurotherm_set_op",
				"Null value received for modbus context");
		return -1;
	}
	/* Update loop mode */
	if (update_euro_loop_mode(mb_context, loop, man_loop) == -1) {
		print_prog_err_msg(PROG_FILE_NAME, "eurotherm_set_op", "Failed to update loop to Manual mode");
		return -1;
	}

	const int reg_value = (int) (out_pow * pow(10, EURO_OUT_POW_RES));
	return write_to_euro_register(mb_context, loop, target_op, reg_value);
}

/* Updates 'target setpoint' for the given loop, changing the 'loop_cntrl_mode' to 'auto_loop'
 *	NOTE: The 'Working Setpoint' is read only, therefore this is how the setpoint must be updated.
 *
 * returns -
 * bytes_w 	: Bytes sent to modbus node
 *  -1		: Error
 * */
int eurotherm_set_sp(const modbus_t * const mb_context, const loop_num loop,
		const double sp) {
	if ((sp < OUT_POW_MIN) || (sp > SP_MAX)) {
		print_prog_err_msg(PROG_FILE_NAME, "eurotherm_set_sp",
				"Setpoint received exceeds allowed range");
		return -1;
	}
	if (!mb_context) {
		print_prog_err_msg(PROG_FILE_NAME, "eurotherm_set_sp",
				"Null value received for modbus context");
		return -1;
	}
	/* Update loop mode */
	if (update_euro_loop_mode(mb_context, loop, auto_loop) == -1) {
		print_prog_err_msg(PROG_FILE_NAME, "eurotherm_set_sp", "Failed to update loop to Auto mode");
		return -1;
	}

	/* Send new sp */
	const int reg_value = (int) (sp * pow(10, EURO_SP_RES));
	return write_to_euro_register(mb_context, loop, target_sp, reg_value);
}

/* TODO Updates the 'euro_vals' object with data from the registers.
 * NOTE: Reads from the wkg_sp and wkg_op registers (these are read only values)
 *
 * returns -
 * 	0	: Success
 * 	-1	: Error
 * */
int eurotherm_get_all(const modbus_t * const mb_context, const loop_num loop,
		const double sp, euro_vals * const euro_vals) {

}

/* Resets the 'euro_vals' to default values. Numerical arrays to -1, booleans to false, and loop control to auto_loop.
 *
 * returns -
 * 0	: Success
 * -1 	: Error, Null value received
 * */
int reset_euro_vals(euro_vals * const euro_vals) {
	if (!euro_vals) {
		print_prog_err_msg(PROG_FILE_NAME, "reset_euro_vals", "NULL value received for euro_vals");
		return -1;
	}
	int i = 0;
	for (i = 0; i < num_loops; i++) {
		euro_vals->active_loops[i] = false;
		euro_vals->cntrl_modes = auto_loop;
		euro_vals->out_pow = -1;
		euro_vals->temp_sps = -1;
		euro_vals->temps = -1;
	}
	return 0;
}

/* TODO Disconnects the given Eurotherm instance, sets 'target op' to 0. */
int automated_eurotherm_shutdown() {
}

/*
 * Finds the next instance for a device in the 'dir' with the substring entered.
 * Prefix must be null terminated (null is not considered during equivalency).
 * NOTE: opendir() must be called prior to the first call of this function.
 *
 * This function can be called sequentially to loop through entirety of directory.
 *
 * returns -
 *   1 	: Match found, 'file' == file of interest
 *   0 	: No matches found, end of directory reached
 * */
int get_next_file(DIR ** const dir, struct dirent **file, char * const substr) {
	*file = readdir(*dir);

	while (*file != NULL ) {
		if (strstr((*file)->d_name, substr) == NULL ) {
			*file = readdir(*dir); /* If no match, view next file in directory */
		} else {
			return 1; /* Matched prefix */
		}
	}
	return 0; /* End of directory reached without match */
}

/*
 * Updates the register that dictates which register (the set point, or output power) is driving the
 * PID loop. man_loop = OP, auto_loop = Setpoint
 *
 * returns :
 * 	bytes_w : Bytes sent to modbus node
 * 		-1	: Error
 * */
static int update_euro_loop_mode(const modbus_t * const mb_context,
		const loop_num loop, const loop_cntrl_mode mode) {
	return write_to_euro_register(mb_context, loop, man_auto_flg, mode);
}

/*
 * Writes the 'reg_value' integer value into the provided 'reg' for the given loop offset.
 *
 * NOTE: How this integer value is determined (in terms of resolution) is set by the configuration
 * of the Eurotherm.
 *
 * returns -
 * bytes_w 	: Number of bytes sent
 * 		-1	: Error
 * */
static int write_to_euro_register(const modbus_t * const mb_context,
		const loop_num loop, const reg_type reg, const int reg_value) {
	int bytes_w = -1;
	const int reg_num = REG_OFFSETS[loop] + reg;

	bytes_w = modbus_write_register(mb_context, reg_num, reg_value);
	if (bytes_w == -1) {
		print_prog_err_msg(PROG_FILE_NAME, "eurotherm_set_sp",
				"Failed to update target sp register");
		return -1;
	}
	return bytes_w;
}
