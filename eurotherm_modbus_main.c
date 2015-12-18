/*
 * eurotherm_modbus_main.c
 *
 *  Created on: Dec 14, 2015
 *      Author: oscott
 */

#include "eurotherm_modbus_subsystem.h"

//
//int main(void) {
//
//	DIR *dir; /* The structure for the dev directory */
//	struct dirent *file; /* The iterative structure for each device in the dev directory */
//	dir = opendir(DIR_ADDR); /* Save off the front of the list, so that multiple traverses can occur to poll with a number of identifiers */
//
//	while (get_next_file(&dir, &file, DEVICE) != 0) {
//		printf("\n%s", file->d_name);
//		fflush(stdout);
//	}
//
////
////	modbus_t *mb;
////	uint16_t tab_reg[32];
////	struct timeval t_o;
////	t_o.tv_sec = 0;
////	t_o.tv_usec = MOD_TO;
////
////	mb = modbus_new_rtu(DEVICE, BAUD_R, 'N', 8, 1);
////	int mode = modbus_rtu_get_serial_mode(mb);
////	modbus_set_response_timeout(mb, &t_o);
////	uint8_t * query = malloc(MODBUS_RTU_MAX_ADU_LENGTH);
////
////	//	  modbus_set_slave(mb, MOD_1);
////	modbus_rtu_set_serial_mode(mb, MODBUS_RTU_RS232);
////
////	modbus_connect(mb);
////	modbus_flush(mb);
////	/* Read 5 registers from the address 0 */
////	modbus_read_registers(mb, 0, 5, tab_reg);
////
////	modbus_close(mb);
////	modbus_free(mb);
//
//	return 0;
//}
