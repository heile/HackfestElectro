/*
 * HF_Flags.c
 *
 *  Created on: Nov 1, 2016
 *      Author: mdube
 */


#include <stdint.h>
#include <HF_player.h>
#include "HF_flags.h"
#include "aes.h"

/*
 * Using Unit Test program. The flags were generated this way:
 *
 * test encrypt 6mYNVO73MLO6Q89uYoFJUMRfmhAYxCN0
 * test encrypt BWBFe9NXeEkh1bsqMjt77WfhtvCPlwFk
 * test encrypt g7D0QUGJs0KnObXW4cBgGHtGUQZTbSNb
 * test encrypt TJixD3YaR5NvEq1EPDMUbpyZ4rJJZbvj
 * test encrypt fOo7vI3reXYDp1p7x9KLun8vIi0KXu6k
 * test encrypt qmApzLwumF2ykFI0fbYkXgsPHx2o8iXU
 * test encrypt CoTOWmje1qUNnXX7goGExhUXcOeyi1mq
 * test encrypt hyN81aRn8w2ZxaWgfDemXZmxNKuil2J9
 * test encrypt MumWs7g5Guwgh7NTlqXwt5C4Xi9nMkg3
 * test encrypt YsD2RK0rlrv7uEGx6YxYAEbYfrg7p7ew
 */

const uint8_t FLAG_1[] = { 0x18, 0x33, 0xd1, 0xd7, 0x2d, 0xe, 0x53, 0xaf, 0x8a, 0x78, 0x61, 0xa0, 0xa0, 0x4e, 0x4, 0xd7,
                  0xf2, 0xa1, 0x96, 0x11, 0x20, 0xb, 0xd7, 0xe, 0x9b, 0x8d, 0xfc, 0xb7, 0x58, 0x2e, 0xbb, 0x3e
                  };
//Decrypted string: 6mYNVO73MLO6Q89uYoFJUMRfmhAYxCN0
const uint8_t FLAG_2[] = { 0x1f, 0xcd, 0x6a, 0x8f, 0x13, 0x56, 0x8a, 0x4, 0x8b, 0x19, 0xdf, 0xd9, 0x4b, 0x1f, 0x8b, 0x8f,
                  0x17, 0xe3, 0x8e, 0xbe, 0xe3, 0xb6, 0x6, 0x30, 0xcc, 0x3, 0xff, 0xea, 0xd8, 0xcd, 0xf6, 0x7b
                  };
//Decrypted string: BWBFe9NXeEkh1bsqMjt77WfhtvCPlwFk
const uint8_t FLAG_3[] = { 0xff, 0x85, 0xf3, 0xc8, 0x50, 0xf7, 0xa6, 0x74, 0x46, 0xd2, 0x81, 0x1a, 0x72, 0xd8, 0x4b, 0x4c,
                  0x4b, 0x28, 0x85, 0x40, 0x97, 0x2, 0x5c, 0x87, 0x28, 0x5, 0x19, 0x95, 0x92, 0xc0, 0x54, 0x27
                  };
//Decrypted string: g7D0QUGJs0KnObXW4cBgGHtGUQZTbSNb
const uint8_t FLAG_4[] = { 0x62, 0x50, 0x1e, 0xc5, 0x4b, 0x9e, 0x5f, 0xe2, 0xc7, 0x18, 0xc8, 0x3f, 0x6b, 0xa6, 0x2b, 0x1,
                  0x9a, 0xc, 0x1e, 0x86, 0x61, 0x1e, 0xc3, 0x3, 0x7c, 0xf4, 0x73, 0x44, 0xdb, 0x38, 0xee, 0xef
                  };
//Decrypted string: TJixD3YaR5NvEq1EPDMUbpyZ4rJJZbvj
const uint8_t FLAG_5[] = { 0x92, 0x2b, 0xe4, 0x5, 0x25, 0xc3, 0x12, 0x68, 0x8, 0x6e, 0x2b, 0x86, 0x6e, 0xb2, 0xd5, 0xf,
                  0xc2, 0x2c, 0x8a, 0xce, 0x54, 0x97, 0x5f, 0xcd, 0x75, 0xee, 0xa3, 0xef, 0xbe, 0xb0, 0x89, 0x77
                  };
//Decrypted string: fOo7vI3reXYDp1p7x9KLun8vIi0KXu6k
const uint8_t FLAG_6[] = { 0x70, 0x25, 0x7b, 0xfe, 0xa4, 0x11, 0xc6, 0xfe, 0x9e, 0xda, 0x7a, 0xac, 0x98, 0x76, 0x43, 0xf7,
                  0xaf, 0x11, 0xf0, 0xfb, 0x90, 0x7a, 0x62, 0xe3, 0xba, 0x4c, 0x67, 0xf4, 0x64, 0x20, 0x80, 0x1
                  };
//Decrypted string: qmApzLwumF2ykFI0fbYkXgsPHx2o8iXU
//const uint8_t FLAG_7[] = { 0x80, 0x3a, 0x13, 0xa0, 0xee, 0x7f, 0xb8, 0x7f, 0x35, 0x10, 0x42, 0x9, 0x73, 0x7d, 0x95, 0x12,
//                  0xc8, 0x9e, 0x71, 0xc5, 0x8, 0xe3, 0x5d, 0x14, 0x1e, 0x2b, 0x2, 0x56, 0x77, 0x24, 0xca, 0xe1
//                  };
const char FLAG_7[] = "Flag: qmApzLwumF2ykFI0fbYkXgsPHx2o8iXU";
//Decrypted string: CoTOWmje1qUNnXX7goGExhUXcOeyi1mq
const uint8_t FLAG_8[] = { 0x29, 0xf1, 0xab, 0x7d, 0x18, 0xf, 0x8f, 0x1b, 0x61, 0x8f, 0x43, 0x69, 0x62, 0xe7, 0x29, 0xf0,
                  0xcd, 0x1f, 0xef, 0x89, 0x96, 0x2b, 0xd4, 0xef, 0x4c, 0x67, 0x7e, 0xd4, 0x2a, 0x16, 0x68, 0xe8
                  };
//Decrypted string: hyN81aRn8w2ZxaWgfDemXZmxNKuil2J9
const uint8_t FLAG_9[] = { 0x58, 0xef, 0xc7, 0xfa, 0xb5, 0xf3, 0xfe, 0x40, 0x50, 0x54, 0xd8, 0xbf, 0x95, 0x66, 0x5f, 0x2f,
                  0x71, 0xa, 0xf9, 0x3d, 0x57, 0x3b, 0x7, 0xb6, 0x1c, 0xeb, 0x44, 0xae, 0x7b, 0x4a, 0x20, 0x5
                  };
//Decrypted string: MumWs7g5Guwgh7NTlqXwt5C4Xi9nMkg3
const uint8_t FLAG_10[] = { 0xe0, 0x5c, 0x12, 0x7b, 0x77, 0xe7, 0x38, 0x49, 0xc0, 0xf4, 0x22, 0x1f, 0x3, 0x3c, 0x2c, 0x34,
                  0xf1, 0x9f, 0x21, 0x3b, 0xc4, 0xd9, 0xde, 0x8, 0xf5, 0xa6, 0x3, 0x2e, 0xf4, 0xab, 0x1, 0xa6
                  };
//Decrypted string: YsD2RK0rlrv7uEGx6YxYAEbYfrg7p7ew


void get_flag(FLAG_NUM flag, char* out){
	switch(flag){
	case FLAG1: decrypt_32byte_str(FLAG_1, out); break;
	case FLAG2: decrypt_32byte_str(FLAG_2, out); break;
	case FLAG3: decrypt_32byte_str(FLAG_3, out); break;
	case FLAG4: decrypt_32byte_str(FLAG_4, out); break;
	case FLAG5: decrypt_32byte_str(FLAG_5, out); break;
	case FLAG6: decrypt_32byte_str(FLAG_6, out); break;
	//case FLAG7: decrypt_32byte_str(FLAG_7, out); break;
	case FLAG8: decrypt_32byte_str(FLAG_8, out); break;
	case FLAG9: decrypt_32byte_str(FLAG_9, out); break;
	case FLAG10: decrypt_32byte_str(FLAG_10, out); break;
	}
}

// THIS FUNCTION MUST BE COMMENTED BEFORE THE CTF
void generate_flags(){}
/*
void generate_flags(){
	char flag1[] = "6mYNVO73MLO6Q89uYoFJUMRfmhAYxCN0";
	char flag2[] = "BWBFe9NXeEkh1bsqMjt77WfhtvCPlwFk";
	char flag3[] = "g7D0QUGJs0KnObXW4cBgGHtGUQZTbSNb";
	char flag4[] = "TJixD3YaR5NvEq1EPDMUbpyZ4rJJZbvj";
	char flag5[] = "fOo7vI3reXYDp1p7x9KLun8vIi0KXu6k";
	char flag6[] = "qmApzLwumF2ykFI0fbYkXgsPHx2o8iXU";
	char flag7[] = "CoTOWmje1qUNnXX7goGExhUXcOeyi1mq";
	char flag8[] = "hyN81aRn8w2ZxaWgfDemXZmxNKuil2J9";
	char flag9[] = "MumWs7g5Guwgh7NTlqXwt5C4Xi9nMkg3";
	char flag10[] = "YsD2RK0rlrv7uEGx6YxYAEbYfrg7p7ew";

	test_encrypt_message(flag1);
	test_encrypt_message(flag2);
	test_encrypt_message(flag3);
	test_encrypt_message(flag4);
	test_encrypt_message(flag5);
	test_encrypt_message(flag6);
	test_encrypt_message(flag7);
	test_encrypt_message(flag8);
	test_encrypt_message(flag9);
	test_encrypt_message(flag10);
}
*/

/*	Team Number */

uint8_t	TEAM_NUM_EEPROM_ADDR;



/* Some flags location in memory */



uint8_t FLAGX_RAM_ADDR;

uint8_t FLAGX_RAM_ADDR;


/*
 * Roles of the different shells
 *
 * USB 		=
 * RS232	=
 * Hacker   =
 */


/*
 * Assign led patterns to events
 *
 * Infinity 		= default
 * Double Infinity 	= Debug mode
 * Double Loop 		= ?
 * Flash			= ?
 */