/*
 * eurotherm_modbus_subsystem.h
 *
 *  Created on: Dec 14, 2015
 *      Author: oscott
 */

#ifndef EURO_MB_SUB_H
#define EURO_MB_SUB_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include<errno.h>
#include<stdbool.h>
#include<math.h>
#include<ctype.h>
#include<unistd.h>
#include<fcntl.h>
#include<termios.h>
#include<dirent.h>
#include<time.h>

#define MAX_EUROTHERMS 3 /* Maximum number of Eurotherms for a single comm port */

#define L1_REGS 1
#define L2_REGS 1025
#define L3_REGS 2049

#define EURO_MOD_BROADCAST 0 /* The ID used for broadcasting on the Euro modbus network */

/*
 * The connection type for a linux system as represented in the /dev/ directory.
 * */
typedef enum serial_port_type{
	tty, ttyUSB, ttyS
}serial_port_type;

/*
 * The control mode for the loop. Defined by the 'setpoint_src' in 'reg_offset'.
 * */
typedef enum loop_cntrl_mode {
	auto_loop = 0, man_loop = 1
} loop_cntrl_mode;

/* Three PID loops per Eurotherm 2704 */
typedef enum loop_num {
	loop_1 = 0, loop_2, loop_3, num_loops
} loop_num;

/* Beginning of relevent registers for each loop */
const int REG_OFFSETS[num_loops] = { L1_REGS, L2_REGS, L3_REGS };

/*
 * These are the register offset values for each loop, as distanced from the PV register value.
 *
 * pv 				- The input temperature for the thermocouple / pyrometer
 * target_sp		- The register that dictates the wkg_sp driving the loop
 * target_op 		- The register that dictates the wkg_op driving the loop
 * wkg_op			- Drives the loop if the loop is in manual mode, else, follows wkg_sp
 * wkg_sp			- Drives the loop if the loop is in auto mode
 * err_val			- Register holding the error code of the loop
 * man_auto_flag 	- Register signifying if the Eurotherm is being driven by Manual (
 *
 * */
typedef enum reg_type {
	pv = 0,
	target_sp = 1,
	target_op = 2,
	wkg_op = 3,
	wkg_sp = 5,
	err_val = 105,
	man_auto_flg = 273
} reg_type;

/*
 * The object storing the modbus network data for a network on a single comm port.
 * */
typedef struct eurotherm_modbus_network{
	modbus_t *mb_context;
	int euro_ids[MAX_EUROTHERMS];		/* Array of eurotherm IDs*/
	bool conn_euros[MAX_EUROTHERMS]; 	/* IDs that are connected */
} euro_mb_network;

/*
 * Structure to define relevant register values as read from the Eurotherm 2704.
 * There shoudld be one structure pure one Eurotherm.
 * */
typedef struct eurotherm_vals {
	bool active_loops[num_loops]; /* Boolean flag to determine if the loop in the loop online */
	loop_cntrl_mode cntrl_modes[num_loops]; /* Cntrl mode for loop */
	double temps[num_loops]; /* Container for the PV register values */
	double temp_sps[num_loops]; /* Container for the wkg_sp vals */
	double out_pow[num_loops];	/* The output power */
} euro_vals;

/* Sniffs the comm ports and searches for a single comm port. Attempts to load modbus slaves with the addresses
 * ranging from start_id to end_id. */
int automated_eurotherms_connection(const serial_port_type port_type, euro_mb_network * const mb_network, const int start_id, const int end_id);

/* Updates 'target op' for the given loop, changing the 'loop_cntrl_mode' to 'man_loop'*/
int eurotherm_set_op(const modbus_t * const mb_context, const loop_num loop,
		const double out_pow);

/* Updates 'target setpoint' for the given loop, changing the 'loop_cntrl_mode' to 'auto_loop'*/
int eurotherm_set_sp(const modbus_t * const mb_context, const loop_num loop,
		const double sp);

/* Updates the 'euro_vals' object with data from the registers.*/
int eurotherm_get_all();

/* Resets the 'euro_vals' to default values. Numerical arrays to -1, booleans to false, and loop control to auto_loop*/
int reset_euro_vals();

/* Disconnects the given Eurotherm instance, sets 'target op' to 0. Invalidates context. */
int automated_eurotherm_shutdown();

#endif
