
/*********************************************/
/*          Include Header                   */
/*********************************************/

#include "include.h"

/**************************************************/

/**************************************************/
/*          Variable                             */
/**************************************************/


void (*transition[2][8])(void) = {
		//Pending
		{STTStop		, STTPendingStateVelocity	,INVALID		, INVALID	,
				INVALID 		, INVALID		, 			INVALID		, INVALID	}	,

				//Velocity
				{STTStop, STTVelocityStateVelocity	, INVALID	, INVALID		,
						INVALID	, 		INVALID	, INVALID			, INVALID	}
};


/**************************************************************/

void STTStateInit(void)
{
	state = RNS_S_PENDING;
}

void STTEventChecker(void)
{

	if(vel1 != 0 || vel2 != 0 || vel3 != 0 || vel4 != 0  ){
		main_board_1_data_receive.common_instruction = RNS_VELOCITY;
		main_board_1_data_receive.common_buffer[0].data=vel1;
		main_board_1_data_receive.common_buffer[1].data=vel2;
		main_board_1_data_receive.common_buffer[2].data=vel3;
		main_board_1_data_receive.common_buffer[3].data=vel4;
		state_flag = 0;
	} else if(state_flag == 0){
		main_board_1_data_receive.common_instruction = RNS_STOP;
		state_flag = 1;
	} else{
		main_board_1_data_receive.common_instruction = RNS_PENDING;
	}

	if (main_board_1_data_receive.common_instruction != RNS_PENDING){

		sys.activate = 0;
		if (main_board_1_data_receive.common_instruction < RNS_INS_RESET)
			event = RNS_E_STOP;
		else if (main_board_1_data_receive.common_instruction < RNS_INS_VELOCITY)
			event = RNS_E_VELOCITY;


		if(transition[state][event] != 0)
			(transition[state][event])();

		sys.activate = 1;
		main_board_1_data_receive.common_instruction = RNS_PENDING;
	}

//	if(APPPrintstatus()){
//				APPPrinting(&uartbuff);
//				UART5_DMA_Send(uartbuff, strlen(uartbuff));
//
//	}
}

void STTStop(void)
{
	APPStop();
	state = RNS_S_PENDING;
}

void STTPendingStateVelocity(void)
{
	ins.instruction = main_board_1_data_receive.common_instruction;
	ins.ins_buffer[0].data = main_board_1_data_receive.common_buffer[0].data;
	ins.ins_buffer[1].data = main_board_1_data_receive.common_buffer[1].data;
	ins.ins_buffer[2].data = main_board_1_data_receive.common_buffer[2].data;
	ins.ins_buffer[3].data = main_board_1_data_receive.common_buffer[3].data;

	APPApply(&ins);
	APPStart(&ins);
	state = RNS_S_VELOCITY;
}

void STTVelocityStateVelocity(void)
{
	ins.instruction = main_board_1_data_receive.common_instruction;
	ins.ins_buffer[0].data = main_board_1_data_receive.common_buffer[0].data;
	ins.ins_buffer[1].data = main_board_1_data_receive.common_buffer[1].data;
	ins.ins_buffer[2].data = main_board_1_data_receive.common_buffer[2].data;
	ins.ins_buffer[3].data = main_board_1_data_receive.common_buffer[3].data;

	APPApply(&ins);
	APPStart(&ins);
	state = RNS_S_VELOCITY;
}




