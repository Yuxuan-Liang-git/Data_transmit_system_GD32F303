/*
 * 说明：将本文添加进工程后，仅需关注 crc_modbus 函数即可，可以直接调用。
 * 		crc_modbus函数作用：输入数组首地址和数组长度，得到校验结果。
*/

#define		CRC_POLY_16			0xA001
#define		CRC_START_MODBUS	0xFFFF

static unsigned char            crc_tab16_init  = 0;
static unsigned short int       crc_tab16[256];

/*
 * static void init_crc16_tab( void );
 *
 * For optimal performance uses the CRC16 routine a lookup table with values
 * that can be used directly in the XOR arithmetic in the algorithm. This
 * lookup table is calculated by the init_crc16_tab() routine, the first time
 * the CRC function is called.
 */

static void init_crc16_tab( void ) {

	unsigned short int i;
	unsigned short int j;
	unsigned short int crc;
	unsigned short int c;

	for (i=0; i<256; i++) {

		crc = 0;
		c   = i;

		for (j=0; j<8; j++) {

			if ( (crc ^ c) & 0x0001 ) crc = ( crc >> 1 ) ^ CRC_POLY_16;
			else                      crc =   crc >> 1;

			c = c >> 1;
		}

		crc_tab16[i] = crc;
	}

	crc_tab16_init = 1;

}  /* init_crc16_tab */

/*
 * unsigned short int crc_modbus( const unsigned char *input_str, unsigned int num_bytes );
 *
 * The function crc_modbus() calculates the 16 bits Modbus CRC in one pass for
 * a byte string of which the beginning has been passed to the function. The
 * number of bytes to check is also a parameter.
 */

static unsigned short int crc_modbus( const unsigned char *input_str, unsigned int num_bytes ) {

	unsigned short int crc;
	const unsigned char *ptr;
	unsigned int a;

	if ( ! crc_tab16_init ) init_crc16_tab();

	crc = CRC_START_MODBUS;
	ptr = input_str;

	if ( ptr != 0 ) for (a=0; a<num_bytes; a++) {

		crc = (crc >> 8) ^ crc_tab16[ (crc ^ (unsigned short int) *ptr++) & 0x00FF ];
	}

	return crc;

}  /* crc_modbus */
