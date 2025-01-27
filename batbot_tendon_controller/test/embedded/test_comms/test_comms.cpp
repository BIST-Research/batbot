#include <Arduino.h>
#include "ml_tendon_commands.hpp"
#include "ml_tendon_comm_protocol.hpp"
#include "unity.h"

TendonController tendons[8] = {
    TendonController("motor 1"),
    TendonController("motor 2"),
    TendonController("motor 3"),
    TendonController("motor 4"),
    TendonController("motor 5"),
    TendonController("motor 6"),
    TendonController("motor 7"),
    TendonController("motor 8")};

void test_packet_validation(void)
{
    // Scenario 1: Test with wrong CRC
    TendonControl_data_packet_s pkt;
    pkt.data_packet_u.data_packet_s.header[0] = 0xFF;
    pkt.data_packet_u.data_packet_s.header[1] = 0x00;
    pkt.data_packet_u.data_packet_s.motorId = 0;
    pkt.data_packet_u.data_packet_s.opcode = 0;
    pkt.data_packet_u.data_packet_s.len = 4;
    pkt.data_packet_u.data_packet_s.pkt_params[0] = 0x00;
    pkt.data_packet_u.data_packet_s.pkt_params[1] = 0x00;

    tendon_comm_result_t result = validatePacket(&pkt);

    TEST_ASSERT_EQUAL(COMM_CRC_ERROR, result);

    // Scenario 2: Test with right CRC
    uint16_t crc = updateCRC(0, pkt.data_packet_u.data_packet, 4 + 3 - TENDON_CONTROL_PKT_NUM_CRC_BYTES);
    pkt.data_packet_u.data_packet_s.pkt_params[0] = TENDON_CONTROL_GET_UPPER_8B(crc);
    pkt.data_packet_u.data_packet_s.pkt_params[1] = TENDON_CONTROL_GET_LOWER_8B(crc);

    result = validatePacket(&pkt);
    TEST_ASSERT_EQUAL(COMM_SUCCESS, result);
}

void test_command_factory1(void)
{
    TendonControl_data_packet_s pkt;
    
    // Scenario 1: Test an invalid opcode
    pkt.data_packet_u.data_packet_s.header[0] = 0xFF;
    pkt.data_packet_u.data_packet_s.header[1] = 0x00;
    pkt.data_packet_u.data_packet_s.motorId = 0;
    pkt.data_packet_u.data_packet_s.opcode = 7;
    pkt.data_packet_u.data_packet_s.len = 4;
    pkt.data_packet_u.data_packet_s.pkt_params[0] = 0x00;
    pkt.data_packet_u.data_packet_s.pkt_params[1] = 0x00;

    ML_TendonCommandBase *cmd = NULL;
    tendon_comm_result_t result = CommandFactory_CreateCommand(
        &cmd,
        &pkt,
        tendons
    );

    TEST_ASSERT_EQUAL(COMM_INSTRUCTION_ERROR, result);
    TEST_ASSERT_EQUAL(NULL, cmd);

    free(cmd);
}

void test_command_factory2(void)
{
    TendonControl_data_packet_s pkt;
    tendons[0].Set_Max_Angle(360);
    tendons[0].Reset_Encoder_Zero();
    tendons[0].Set_PID_Param(1, 0, 0, 5000);
    
    // Scenario 1: Test a valid write motor command packet
    pkt.data_packet_u.data_packet_s.header[0] = 0xFF;
    pkt.data_packet_u.data_packet_s.header[1] = 0x00;
    pkt.data_packet_u.data_packet_s.motorId = 0;
    pkt.data_packet_u.data_packet_s.opcode = WRITE_ANGLE;
    pkt.data_packet_u.data_packet_s.len = 5;
    pkt.data_packet_u.data_packet_s.pkt_params[0] = 50;

    ML_TendonCommandBase *cmd = NULL;
    tendon_comm_result_t result = CommandFactory_CreateCommand(
        &cmd,
        &pkt,
        tendons
    );

    TEST_ASSERT_EQUAL(COMM_SUCCESS, result);
    TEST_ASSERT_NOT_NULL(cmd);
    TEST_ASSERT_EQUAL(ML_WriteAngleCommand_execute, ((ML_WriteAngleCommand*)cmd)->base.fn);
    TEST_ASSERT_EQUAL(&tendons[0], ((ML_WriteAngleCommand*)cmd)->base.motor_ref);
    TEST_ASSERT_EQUAL(50, ((ML_WriteAngleCommand*)cmd)->anglePercent);

    // Scenario 2: Assert that the motors goal angle has been updated to the set goal angle
    CommandReturn_t cmd_ret = cmd->fn(cmd);
    TEST_ASSERT_EQUAL(0, cmd_ret.numParams);
    TEST_ASSERT_FLOAT_WITHIN(1, 180, tendons[0].Get_Goal_Angle());
    
    // Scenario 3: Try to exceed max angle
    pkt.data_packet_u.data_packet_s.pkt_params[0] = 110;
    result = CommandFactory_CreateCommand(
        &cmd,
        &pkt,
        tendons
    );
    TEST_ASSERT_EQUAL(COMM_SUCCESS, result);
    TEST_ASSERT_NOT_NULL(cmd);
    TEST_ASSERT_EQUAL(ML_WriteAngleCommand_execute, ((ML_WriteAngleCommand*)cmd)->base.fn);
    TEST_ASSERT_EQUAL(&tendons[0], ((ML_WriteAngleCommand*)cmd)->base.motor_ref);
    TEST_ASSERT_EQUAL(110, ((ML_WriteAngleCommand*)cmd)->anglePercent);
    cmd_ret = cmd->fn(cmd);
    TEST_ASSERT_EQUAL(0, cmd_ret.numParams);
    TEST_ASSERT_FLOAT_WITHIN(1, 360, tendons[0].Get_Goal_Angle());

    free(cmd);
}

void test_command_factory3(void)
{
    // Scenario 1: Test a valid set max angle command packet
    TendonControl_data_packet_s pkt;
    
    pkt.data_packet_u.data_packet_s.header[0] = 0xFF;
    pkt.data_packet_u.data_packet_s.header[1] = 0x00;
    pkt.data_packet_u.data_packet_s.motorId = 0;
    pkt.data_packet_u.data_packet_s.opcode = SET_ZERO_ANGLE;
    pkt.data_packet_u.data_packet_s.len = 6;
    pkt.data_packet_u.data_packet_s.pkt_params[0] = TENDON_CONTROL_GET_UPPER_8B(100);
    pkt.data_packet_u.data_packet_s.pkt_params[1] = TENDON_CONTROL_GET_LOWER_8B(100);

    ML_TendonCommandBase *cmd = NULL;
    tendon_comm_result_t result = CommandFactory_CreateCommand(
        &cmd,
        &pkt,
        tendons
    );
    TEST_ASSERT_EQUAL(COMM_SUCCESS, result);
    TEST_ASSERT_NOT_NULL(cmd);
    TEST_ASSERT_EQUAL(ML_SetZeroAngleCommand_execute, ((ML_SetZeroAngleCommand*)cmd)->base.fn);
    TEST_ASSERT_EQUAL(&tendons[0], ((ML_SetZeroAngleCommand*)cmd)->base.motor_ref);

    // Scenario 2: Assert that the motors zero angle has been reset
    CommandReturn_t cmd_ret = cmd->fn(cmd);
    TEST_ASSERT_EQUAL(0, cmd_ret.numParams);
    TEST_ASSERT_FLOAT_WITHIN(0.01, 0, tendons[0].Get_Angle());
    TEST_ASSERT_FLOAT_WITHIN(0.01, 0, tendons[0].Get_Goal_Angle());

    free(cmd);
}

void test_command_factory4(void)
{
    // Scenario 1: Test a valid set zero angle command packet
    TendonControl_data_packet_s pkt;
    
    pkt.data_packet_u.data_packet_s.header[0] = 0xFF;
    pkt.data_packet_u.data_packet_s.header[1] = 0x00;
    pkt.data_packet_u.data_packet_s.motorId = 0;
    pkt.data_packet_u.data_packet_s.opcode = SET_MAX_ANGLE;
    pkt.data_packet_u.data_packet_s.len = 6;
    pkt.data_packet_u.data_packet_s.pkt_params[0] = TENDON_CONTROL_GET_UPPER_8B(100);
    pkt.data_packet_u.data_packet_s.pkt_params[1] = TENDON_CONTROL_GET_LOWER_8B(100);
    

    ML_TendonCommandBase *cmd = NULL;
    tendon_comm_result_t result = CommandFactory_CreateCommand(
        &cmd,
        &pkt,
        tendons
    );

    TEST_ASSERT_EQUAL(COMM_SUCCESS, result);
    TEST_ASSERT_NOT_NULL(cmd);
    TEST_ASSERT_EQUAL(ML_SetMaxAngleCommand_execute, ((ML_SetMaxAngleCommand*)cmd)->base.fn);
    TEST_ASSERT_EQUAL(&tendons[0], ((ML_SetMaxAngleCommand*)cmd)->base.motor_ref);

    // Scenario 2: Assert that the max angle has been set
    CommandReturn_t cmd_ret = cmd->fn(cmd);
    TEST_ASSERT_EQUAL(0, cmd_ret.numParams);
    TEST_ASSERT_FLOAT_WITHIN(0.01, 100, tendons[0].Get_Max_Angle());

    free(cmd);
}

void test_command_factory5(void)
{
    tendons[0].Reset_Encoder_Zero();
    
    // Scenario 1: Test a valid read angle command packet
    TendonControl_data_packet_s pkt;
    
    pkt.data_packet_u.data_packet_s.header[0] = 0xFF;
    pkt.data_packet_u.data_packet_s.header[1] = 0x00;
    pkt.data_packet_u.data_packet_s.motorId = 0;
    pkt.data_packet_u.data_packet_s.opcode = READ_ANGLE;
    pkt.data_packet_u.data_packet_s.len = 4;

    ML_TendonCommandBase *cmd = NULL;
    tendon_comm_result_t result = CommandFactory_CreateCommand(
        &cmd,
        &pkt,
        tendons
    );

    TEST_ASSERT_EQUAL(COMM_SUCCESS, result);
    TEST_ASSERT_NOT_NULL(cmd);
    TEST_ASSERT_EQUAL(ML_ReadAngleCommand_execute, ((ML_ReadAngleCommand*)cmd)->base.fn);
    TEST_ASSERT_EQUAL(&tendons[0], ((ML_ReadAngleCommand*)cmd)->base.motor_ref);

    // Scenario 2: Assert that the correct angle has been read
    CommandReturn_t cmd_ret = cmd->fn(cmd);

    TEST_ASSERT_EQUAL(2, cmd_ret.numParams);
    TEST_ASSERT_EQUAL(0, TENDON_CONTROL_MAKE_16B_WORD(cmd_ret.params[0], cmd_ret.params[1]));

    // Scenario 3: Test with negative angle
    tendons[0].Set_Angle(-90);
    cmd_ret = cmd->fn(cmd);

    TEST_ASSERT_EQUAL(2, cmd_ret.numParams);
    Serial.println(cmd_ret.params[0]);
    Serial.println(cmd_ret.params[1]);
    TEST_ASSERT_INT_WITHIN(2, (uint16_t)(-90), TENDON_CONTROL_MAKE_16B_WORD(cmd_ret.params[0], cmd_ret.params[1]));

    free(cmd);
}

void test_build_response_packet1(void)
{
    
}

void test_packet_handling(void)
{
    // Scenario 1: Test packet handling without CRC
    TendonControl_data_packet_s pkt;
    
    // pkt.data_packet_u.data_packet_s.header[0] = 0xFF;
    // pkt.data_packet_u.data_packet_s.header[1] = 0x00;
    // pkt.data_packet_u.data_packet_s.motorId = 0;
    // pkt.data_packet_u.data_packet_s.opcode = SET_MAX_ANGLE;
    // pkt.data_packet_u.data_packet_s.len = 6;
    // pkt.data_packet_u.data_packet_s.pkt_params[0] = TENDON_CONTROL_GET_UPPER_8B(100);
    // pkt.data_packet_u.data_packet_s.pkt_params[1] = TENDON_CONTROL_GET_LOWER_8B(100);


    // TendonControl_data_packet_s return_pkt = handlePacket((const char*)pkt.data_packet_u.data_packet, tendons);
    // TEST_ASSERT_EQUAL(READ_STATUS, return_pkt.data_packet_u.data_packet_s.opcode);
    // TEST_ASSERT_EQUAL(COMM_CRC_ERROR, return_pkt.data_packet_u.data_packet_s.pkt_params[0]);

    // Scenario 2: Test packet handling with CRC
    // pkt.data_packet_u.data_packet_s.header[0] = 0xFF;
    // pkt.data_packet_u.data_packet_s.header[1] = 0x00;
    // pkt.data_packet_u.data_packet_s.motorId = 0;
    // pkt.data_packet_u.data_packet_s.opcode = SET_MAX_ANGLE;
    // pkt.data_packet_u.data_packet_s.len = 6;
    // pkt.data_packet_u.data_packet_s.pkt_params[0] = TENDON_CONTROL_GET_UPPER_8B(100);
    // pkt.data_packet_u.data_packet_s.pkt_params[1] = TENDON_CONTROL_GET_LOWER_8B(100);
    // uint16_t crc = updateCRC(0, pkt.data_packet_u.data_packet, 3 + 6 - TENDON_CONTROL_PKT_NUM_CRC_BYTES);
    // pkt.data_packet_u.data_packet_s.pkt_params[2] = TENDON_CONTROL_GET_UPPER_8B(crc);
    // pkt.data_packet_u.data_packet_s.pkt_params[3] = TENDON_CONTROL_GET_LOWER_8B(crc);

    // return_pkt = handlePacket((const char*)pkt.data_packet_u.data_packet, tendons);
    // TEST_ASSERT_EQUAL(READ_STATUS, return_pkt.data_packet_u.data_packet_s.opcode);

    // TEST_ASSERT_EQUAL(COMM_SUCCESS, return_pkt.data_packet_u.data_packet_s.pkt_params[0]);
    // TEST_ASSERT_FLOAT_WITHIN(0.01, 100, tendons[0].Get_Max_Angle());

    // Scenario 3: Test read angle
    tendons[0].Reset_Encoder_Zero();
    pkt.data_packet_u.data_packet_s.header[0] = 0xFF;
    pkt.data_packet_u.data_packet_s.header[1] = 0x00;
    pkt.data_packet_u.data_packet_s.motorId = 0;
    pkt.data_packet_u.data_packet_s.opcode = READ_ANGLE;
    pkt.data_packet_u.data_packet_s.len = 4;
    uint16_t crc = updateCRC(0, pkt.data_packet_u.data_packet, 3 + 4 - TENDON_CONTROL_PKT_NUM_CRC_BYTES);
    pkt.data_packet_u.data_packet_s.pkt_params[0] = TENDON_CONTROL_GET_UPPER_8B(crc);
    pkt.data_packet_u.data_packet_s.pkt_params[1] = TENDON_CONTROL_GET_LOWER_8B(crc);

    TendonControl_data_packet_s return_pkt = handlePacket((const char*)pkt.data_packet_u.data_packet, tendons);
    TEST_ASSERT_EQUAL(READ_STATUS, return_pkt.data_packet_u.data_packet_s.opcode);
    TEST_ASSERT_EQUAL(COMM_SUCCESS, return_pkt.data_packet_u.data_packet_s.pkt_params[0]);

    for (int i = 0; i < 3 + return_pkt.data_packet_u.data_packet_s.len; ++i)
    {
        Serial.println(return_pkt.data_packet_u.data_packet[i]);
    }
    uint16_t angle = TENDON_CONTROL_MAKE_16B_WORD(return_pkt.data_packet_u.data_packet_s.pkt_params[1], return_pkt.data_packet_u.data_packet_s.pkt_params[2]);
    TEST_ASSERT_EQUAL(0.0, angle);
}

void setup() {
    delay(5000);

    UNITY_BEGIN();
    RUN_TEST(test_packet_validation);
    RUN_TEST(test_command_factory1);
    RUN_TEST(test_command_factory2);
    RUN_TEST(test_command_factory3);
    RUN_TEST(test_command_factory4);
    RUN_TEST(test_command_factory5);
    RUN_TEST(test_packet_handling);
    UNITY_END();
}

void loop() {
    
}