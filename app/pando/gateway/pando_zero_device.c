/*******************************************************
 * File name: pando_zero_device.c
 * Author:  Chongguang Li
 * Versions: 1.0
 * Description:This module is used to process the gateway business.
 * History:
 *   1.Date:
 *     Author:
 *     Modification:
 *********************************************************/
#include "pando_channel.h"
#include "../protocol/sub_device_protocol.h"
#include "pando_system_time.h"
#include "pando_zero_device.h"
#include "user_interface.h"

#define COMMON_COMMAND_UPGRADE 65529
#define COMMON_COMMAND_REBOOT  65535
#define COMMON_COMMAND_SYN_TIME 65531

/******************************************************************************
 * FunctionName : zero_device_data_process.
 * Description  : process the received data of zero device(zero device is the gateway itself).
 * Parameters   : uint.
 * Returns      : none.
*******************************************************************************/
static void ICACHE_FLASH_ATTR
zero_device_data_process(uint8_t * buffer, uint16_t length)
{
	struct pando_command cmd_body;
	uint16_t type = 0;
	uint16_t len = 0;
	struct sub_device_buffer * device_buffer = (struct sub_device_buffer *)os_malloc(sizeof(struct sub_device_buffer));
	if(device_buffer == NULL)
	{
		PRINTF("%s:malloc error!\n", __func__);
		return;
	}
	device_buffer->buffer = (uint8*)os_malloc(length);
	if(device_buffer->buffer == NULL)
	{
		PRINTF("%s:malloc error!\n", __func__);
		return;
	}
	os_memcpy(device_buffer->buffer, buffer, length);
	device_buffer->buffer_length = length;

	struct TLVs *cmd_param = get_sub_device_command(device_buffer, &cmd_body);
	if(COMMON_COMMAND_SYN_TIME == cmd_body.command_id )
	{
		PRINTF("PANDO: synchronize time\n");
		uint64 time = get_next_uint64(cmd_param);
	    show_package((uint8*)(&time), sizeof(time));
	    pando_set_system_time(time);
	}

}

/******************************************************************************
 * FunctionName : ipando_zero_device_init
 * Description  : initialize the zero device(zero device is the gateway itself).
 * Parameters   : none.
 * Returns      : none.
*******************************************************************************/
void ICACHE_FLASH_ATTR
pando_zero_device_init(void)
{
	on_subdevice_channel_recv(PANDO_CHANNEL_PORT_0, zero_device_data_process);
}
