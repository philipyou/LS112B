#ifndef __BTREG_H
#define __BTREG_H

#define mem_eir				((volatile u8_t xdata *)0x01bc)
#define mem_module_uuid_list	((volatile u8_t xdata *)0x48a0)

#define mem_poweron_flag	((volatile u8_t xdata *)0x49fc)
#define CS_NULL_FLAG		0x00
#define CS_POWERON_FLAG	0x10
#define CS_IOWAKE_FLAG		0x16

#define mem_lpm_lock	((volatile u8_t xdata *)0x49fe)
#define NOT_ENTER_LPM 	0
#define ENTER_LPM		1
#define LPM_ENABLE()		*mem_lpm_lock = ENTER_LPM
#define LPM_DISABLE()	*mem_lpm_lock = NOT_ENTER_LPM

#define mem_ui_state_map	((volatile u8_t xdata *)0x47f4)
#define UI_STATE_BLE_CONNECTED	(1<<9)

#define	mem_le_mac		((volatile u8_t xdata *)0x43e8)
#define	mem_le_adv_data	((volatile u8_t xdata *)0x43da)
#define	mem_le_lap		((volatile u8_t xdata *)0x44f9) 
#define  	mem_adv_lap_ptr	((volatile u16_t xdata *)0x49e2) 

#define	mem_cmd_rptr				0x4a00 
#define	mem_cmd_wptr				0x4a02 
#define	mem_event_rptr				0x4a04 
#define	mem_event_wptr				0x4a06 

#define	mem_cmd_buf                             	0x4f00
#define	mem_cmd_buf_end                         	0x4f7f
#define	mem_event_buf                           	0x4f80
#define	mem_event_buf_end                       0x4fff


#define 	mem_spiflash_buf				((volatile u8_t xdata *)0x4c00)
#define	mem_sensor_buf				(mem_spiflash_buf + 0x08)
#define 	mem_register_buf				(mem_spiflash_buf + 0x0e)
#define	mem_user_buf				(mem_spiflash_buf + 0x19)	
 
#endif



