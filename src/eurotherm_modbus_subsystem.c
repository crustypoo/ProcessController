/*
 * eurotherm_modbus_subsystem.c
 *
 *  Created on: Dec 14, 2015
 *      Author: oscott
 */


#include "eurotherm_modbus_subsystem.h"
#include <modbus.h>
#include <dirent.h> 		/* needed for memset function */
#include <stdlib.h>         /* NULL */
#include <stdio.h>			/* printf, sprintf, memset functions */
#include <string.h>

#define MOD_TO 50000
#define BAUD_R 19200
#define DIR_ADDR "/dev/"
#define DEVICE "tty"
#define BROADCAST_ID 0	/* No reply during broadcast communications */

/* Sniffs the comm ports and searches for a single comm port. Attempts to load modbus slaves with the addresses
 * ranging from start_id to end_id.
 * */
int automated_eurotherms_connection(const serial_port_type port_type, euro_mb_network * const mb_network, const int start_id, const int end_id);

/* Updates 'target op' for the given loop, changing the 'loop_cntrl_mode' to 'man_loop'*/
int eurotherm_set_op();

/* Updates 'target setpoint' for the given loop, changing the 'loop_cntrl_mode' to 'auto_loop'*/
int eurotherm_set_sp(const loop_num, const double sp);

/* Updates the 'euro_vals' object with data from the registers.*/
int eurotherm_get_all();

/* Resets the 'euro_vals' to default values. Numerical arrays to -1, booleans to false, and loop control to auto_loop*/
int reset_euro_vals();

/* Disconnects the given Eurotherm instance, sets 'target op' to 0. */
int automated_eurotherm_shutdown();

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

int main(void) {

	DIR *dir; /* The structure for the dev directory */
	struct dirent *file; /* The iterative structure for each device in the dev directory */
	dir = opendir(DIR_ADDR); /* Save off the front of the list, so that multiple traverses can occur to poll with a number of identifiers */

	while(get_next_file(&dir, &file, DEVICE) != 0) {
		printf("\n%s", file->d_name);
		fflush(stdout);
	}


//
//	modbus_t *mb;
//	uint16_t tab_reg[32];
//	struct timeval t_o;
//	t_o.tv_sec = 0;
//	t_o.tv_usec = MOD_TO;
//
//	mb = modbus_new_rtu(DEVICE, BAUD_R, 'N', 8, 1);
//	int mode = modbus_rtu_get_serial_mode(mb);
//	modbus_set_response_timeout(mb, &t_o);
//	uint8_t * query = malloc(MODBUS_RTU_MAX_ADU_LENGTH);
//
//	//	  modbus_set_slave(mb, MOD_1);
//	modbus_rtu_set_serial_mode(mb, MODBUS_RTU_RS232);
//
//	modbus_connect(mb);
//	modbus_flush(mb);
//	/* Read 5 registers from the address 0 */
//	modbus_read_registers(mb, 0, 5, tab_reg);
//
//	modbus_close(mb);
//	modbus_free(mb);

	return 0;
}
