/**
 * Maximum Power Point Tracker Project
 *
 * File: CanIdList.cpp
 * Author: Matthew Yu
 * Organization: UT Solar Vehicles Team
 * Created on: May 25th, 2021
 * Last Modified: 05/25/21
 *
 * File Description: This file defines valid CAN IDs for the Array/MPPT system.
 */
#pragma once
#define i_MPPT_1_ARR_V_SP       0x600
#define o_MPPT_1_ARR_V_MEAS     0x601
#define o_MPPT_1_ARR_C_MEAS     0x602
#define o_MPPT_1_BATT_V_MEAS    0x603
#define o_MPPT_1_BATT_C_MEAS    0x604
#define i_MPPT_1_EN_DIS         0x605
#define o_MPPT_1_FAULT          0x606

#define i_MPPT_2_ARR_V_SP       0x610
#define o_MPPT_2_ARR_V_MEAS     0x611
#define o_MPPT_2_ARR_C_MEAS     0x612
#define o_MPPT_2_BATT_V_MEAS    0x613
#define o_MPPT_2_BATT_C_MEAS    0x614
#define i_MPPT_2_EN_DIS         0x615
#define o_MPPT_2_FAULT          0x616

#define RTD_TEMP_MEAS           0x620
#define IRRAD_1_MEAS            0x630
#define IRRAD_2_MEAS            0x631
#define RTD_IRRAD_EN_DIS        0x632
#define RTD_IRRAD_FAULT         0x633

#define INVALID_CAN_ID          0xFFFF