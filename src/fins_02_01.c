/*
 * Library: libfins
 * File:    src/fins_02_01.c
 * Author:  Lammert Bies
 *
 * This file is licensed under the MIT License as stated below
 *
 * Copyright (c) 2016 Lammert Bies
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * Description
 * -----------
 * The source file src/fins_02_01.c contains routines to read data from the
 * parameter area of a remote PLC over the FINS protocol.
 */

#include "fins.h"

/*
 * int finslib_parameter_area_read( struct fins_sys_tp *sys, uint16_t area_code, uint16_t *data, uint16_t start_word, size_t num_words );
 *
 * The function finslib_parameter_area_read() reads a block of data from a
 * parameter area of a remote PLC over the FINS protocol.
 *
 * The function returns a success or error code from the list FINS_RETVAL_...
 */

int finslib_parameter_area_read( struct fins_sys_tp *sys, uint16_t area_code, uint16_t *data, uint16_t start_word, size_t num_words ) {

	struct fins_command_tp fins_cmnd;
	size_t a;
	size_t bodylen;
	int retval;

	if ( num_words   == 0              ) return FINS_RETVAL_SUCCESS;
	if ( num_words   >  498            ) return FINS_RETVAL_BODY_TOO_LONG;
	if ( sys         == NULL           ) return FINS_RETVAL_NOT_INITIALIZED;
	if ( data        == NULL           ) return FINS_RETVAL_NO_DATA_BLOCK;
	if ( sys->sockfd == INVALID_SOCKET ) return FINS_RETVAL_NOT_CONNECTED;

	if ( area_code != FINS_PARAM_AREA_PLC_SETUP              &&
	     area_code != FINS_PARAM_AREA_IO_TABLE_REGISTRATION  &&
	     area_code != FINS_PARAM_AREA_ROUTING_TABLE          &&
	     area_code != FINS_PARAM_AREA_CPU_BUS_UNIT_SETUP          ) return FINS_RETVAL_INVALID_PARAMETER_AREA;

	fins_init_command( sys, & fins_cmnd, 0x02, 0x01 );

	bodylen = 0;

	fins_cmnd.body[bodylen++] = (area_code  >> 8) & 0xff;
	fins_cmnd.body[bodylen++] = (area_code      ) & 0xff;
	fins_cmnd.body[bodylen++] = (start_word >> 8) & 0xff;
	fins_cmnd.body[bodylen++] = (start_word     ) & 0xff;
	fins_cmnd.body[bodylen++] = (num_words  >> 8) & 0xff;
	fins_cmnd.body[bodylen++] = (num_words      ) & 0xff;

	if ( ( retval = _finslib_communicate( sys, & fins_cmnd, & bodylen ) ) != FINS_RETVAL_SUCCESS ) return retval;

	if ( bodylen != 8+2*num_words ) return FINS_RETVAL_BODY_TOO_SHORT;

	bodylen = 8;

	for (a=0; a<num_words; a++) {

		data[a]   = fins_cmnd.body[bodylen++];
		data[a] <<= 8;
		data[a]  += fins_cmnd.body[bodylen++];
	}

	return FINS_RETVAL_SUCCESS;

}  /* finslib_parameter_area_read */