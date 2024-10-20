
/////////////////////
//			       //
//  KBC registers  //
//				   //
/////////////////////

#define		 KBC_IRQ_KBD  0x01 //keyboard IRQ id
#define    KBC_IRQ_MOUSE  0x0C //mouse IRQ id

#define   KBC_REG_IN_BUF  0x60 //write argument byte to KBC
#define  KBC_REG_OUT_BUF  0x60 //read out buffer
#define	    KBC_REG_STAT  0x64 //read KBC status
#define  KBC_REG_COMMAND  0x64 //write commands to KBC



///////////////////////
//			         //
//  KBC status bits  //
//				     //
///////////////////////

#define  KBC_STAT_PARITY  BIT(7) //if 1 invalid data
#define KBC_STAT_TIMEOUT  BIT(6) //if 1 invalid data
#define		KBC_STAT_AUX  BIT(5) //if 1 mouse data
#define		KBC_STAT_INH  BIT(4) //if 0 KB inhibited
#define		 KBC_STAT_A2  BIT(3) //A2 input (not using)
#define		KBC_STAT_SYS  BIT(2) //sys flag
#define		KBC_STAT_IBF  BIT(1) //if 1 IN_BUF full (dont write)
#define		KBC_STAT_OBF  BIT(0) //if 1 OUT_BUF ready (can read)



////////////////////
//			      //
//  KBC commands  //
//				  //
////////////////////

#define  KBC_COMM_READ_BYTE  0x20 //read command byte
#define KBC_COMM_WRITE_BYTE  0x60 //write commandbyte

#define   KBC_COMM_DISABLE_KBD_INTERFACE  0xAD //disable keyboard interface
#define    KBC_COMM_ENABLE_KBD_INTERFACE  0xAE //enable keyboard interface
#define KBC_COMM_DISABLE_MOUSE_INTERFACE  0xA7 //disable mouse interface
#define  KBC_COMM_ENABLE_MOUSE_INTERFACE  0xA8 //enable mouse interface

#define   KBC_COMM_WRITE_MOUSE_COMM_BYTE  0xD4 //write command to mouse



//////////////////////
//			        //
//  Mouse commands  //
//			   	    //
//////////////////////

//arguments//
#define MOUSE_COMM_DISABLE_DATA_REPORTING  0xF5 //disable data reporting
#define  MOUSE_COMM_ENABLE_DATA_REPORTING  0xF4 //enable data reporting
#define MOUSE_COMM_READ_DATA 0xEB // read data

//responses to command//
#define   MOUSE_RES_ACK  0xFA //command accepted
#define  MOUSE_RES_NACK  0xFE //command not accepted
#define MOUSE_RES_ERROR  0xFC //command not accepted



////////////////////////
//					  //
//  KBC command bits  //
//					  //
////////////////////////

#define KBC_COMM_B_DIS_MOUSE  BIT(5) //disable mouse interface
#define   KBC_COMM_B_DIS_KBD  BIT(4) // disable keyboard interface
#define KBC_COMM_B_INT_MOUSE  BIT(1) // enable mouse interrupts
#define   KBC_COMM_B_INT_KBD  BIT(0) // enable keyboard interrupts



/////////////////////////
//					   //
//  Mouse Data Packet  //
//					   //
/////////////////////////

#define MOUSE_DP_BYTE_1  0x01 //data pack byte with information below
#define MOUSE_DP_BYTE_2  0x02 //x delta
#define MOUSE_DP_BYTE_3  0x03 //y delta



//////////////////////////////
//					        //
//  Mouse Data Packet bits  //
//					        //
//////////////////////////////
		 
#define MOUSE_DP_Y_OVFL  BIT(7) //1 if y delta overflow
#define MOUSE_DP_X_OVFL  BIT(6) //1 if x delta overflow

#define MOUSE_DP_MSB_Y  BIT(5) //y delta MSB
#define MOUSE_DP_MSB_X  BIT(4) //x delta MSB

#define MOUSE_DP_FIRST_B  BIT(3) //1 if reading first packet byte

#define MOUSE_DP_MB  BIT(2) //1 if middle button pressed
#define MOUSE_DP_RB  BIT(1) //1 if right button pressed
#define	MOUSE_DP_LB  BIT(0) //1 if left button pressed



/////////////////
//	     	   //
//  Key codes  //
//		       //
/////////////////

#define  W_MAKECODE 0x11
#define	W_BREAKCODE 0x91
#define	 A_MAKECODE 0x1e
#define A_BREAKCODE 0x9e
#define	 S_MAKECODE 0x1f
#define	S_BREAKCODE 0x9f
#define	 D_MAKECODE 0x20
#define	D_BREAKCODE 0xa0

#define	   ESC_MAKECODE 0x01
#define	  ESC_BREAKCODE 0x81
#define  SHIFT_MAKECODE 0x2a
#define SHIFT_BREAKCODE 0xaa



/////////////
//		   //
//  OTHER  //
//		   //
/////////////

#define   DOUBLE_BYTE  0xE0   //1st byte of 2 byte scancodes
#define		BREAK_BIT  BIT(7) //break code bit

#define	    DELAY_US  20000 //20 ms delay
#define	MAX_ATTEMPTS  0x14  //max attempts


