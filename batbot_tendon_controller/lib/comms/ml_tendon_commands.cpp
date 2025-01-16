#include "ml_tendon_commands.hpp"

CommandReturn_t ML_EchoCommand_execute(struct ML_EchoCommand * self)
{
    return CommandReturn_t{
        self->numParams,
        self->params
    };
}

CommandReturn_t ML_ReadStatusCommand_execute(struct ML_ReadStatusCommand * self)
{
    return CommandReturn_t{
        1,
        {0}
    };
}

CommandReturn_t ML_ReadAngleCommand_execute(struct ML_ReadAngleCommand * self)
{
    uint16_t angle = (uint16_t)(self->base.motor_ref->Get_Angle());

    uint8_t angle_h = (angle >> 8) & 0xFF;
    uint8_t angle_l = (angle) & 0xFF;

    uint8_t params[] = {angle_h, angle_l};

    return CommandReturn_t{
        2,
        params
    };
}

CommandReturn_t ML_WriteAngleCommand_execute(struct ML_WriteAngleCommand * self)
{
    float angleToWrite = self->base.motor_ref->Get_Max_Angle() * (float)self->anglePercent / 100;
    self->base.motor_ref->Set_Goal_Angle(angleToWrite);

    return CommandReturn_t{
        0,
        {}
    };
}

CommandReturn_t ML_WRitePIDCommand_execute(struct ML_WritePIDCommand * self)
{
    // TODO

    return CommandReturn_t {
        0,
        {}
    };
}

CommandReturn_t ML_SetZeroAngleCommand_execute(struct ML_SetZeroAngleCommand * self)
{
    self->base.motor_ref->Reset_Encoder_Zero();

    return CommandReturn_t {
        0,
        {}
    };
}

CommandReturn_t ML_SetMaxAngleCommand_execute(struct ML_SetMaxAngleCommand * self)
{
    self->base.motor_ref->Set_Max_Angle(self->angle);

    return CommandReturn_t {
        0,
        {}
    };
}

tendon_comm_result_t ML_EchoCommand_create(
    ML_TendonCommandBase* command,
    TendonControl_data_packet_s* dataPacket,
    TendonController* tendons
)
{
    command = (ML_TendonCommandBase *)(new ML_EchoCommand);
    command->fn = (CommandExecuteFn)ML_EchoCommand_execute;
    return COMM_SUCCESS;
}

tendon_comm_result_t ML_ReadStatusCommand_create(
    ML_TendonCommandBase* command,
    TendonControl_data_packet_s* dataPacket,
    TendonController* tendons
)
{
    uint8_t id = dataPacket->data_packet_u.data_packet_s.motorId;

    if (id >= 8) // TODO: Move the definition of the motor number somewhere thats visible to this file
    {
        return COMM_ID_ERROR;
    } else {
        command = (ML_TendonCommandBase *)(new ML_ReadStatusCommand);
        command->motor_ref = &tendons[id];
        command->fn = (CommandExecuteFn)ML_ReadAngleCommand_execute;

        return COMM_SUCCESS;
    }
}

tendon_comm_result_t ML_ReadAngleCommand_create(
    ML_TendonCommandBase* command,
    TendonControl_data_packet_s* dataPacket,
    TendonController* tendons
)
{
    uint8_t id = dataPacket->data_packet_u.data_packet_s.motorId;

    if (id >= 8) // TODO: Move the definition of the motor number somewhere thats visible to this file
    {
        return COMM_ID_ERROR;
    } else {
        command = (ML_TendonCommandBase *)(new ML_ReadAngleCommand);
        command->motor_ref = &tendons[id];
        command->fn = (CommandExecuteFn)ML_ReadAngleCommand_execute;

        return COMM_SUCCESS;
    }
}

tendon_comm_result_t ML_WriteAngleCommand_create(
    ML_TendonCommandBase* command,
    TendonControl_data_packet_s* dataPacket,
    TendonController* tendons
)
{
    uint8_t id = dataPacket->data_packet_u.data_packet_s.motorId;
    size_t numParams = dataPacket->data_packet_u.data_packet_s.len - 4;

    if (id >= 8) // TODO: Move the definition of the motor number somewhere thats visible to this file
    {
        return COMM_ID_ERROR;
    } else if (numParams != 2) {
        return COMM_PARAM_ERROR;
    }
    else {
        command = (ML_TendonCommandBase *)(new ML_WriteAngleCommand);
        command->motor_ref = &tendons[id];
        command->fn = (CommandExecuteFn)ML_WriteAngleCommand_execute;

        ((ML_WriteAngleCommand *)command)->anglePercent = (int)(dataPacket->data_packet_u.data_packet_s.pkt_params[0]);

        return COMM_SUCCESS;
    }
}

tendon_comm_result_t ML_WritePIDCommand_create(
    ML_TendonCommandBase* command,
    TendonControl_data_packet_s* dataPacket,
    TendonController* tendons
)
{
    uint8_t id = dataPacket->data_packet_u.data_packet_s.motorId;
    size_t numParams = dataPacket->data_packet_u.data_packet_s.len - 4;

    if (id >= 8) // TODO: Move the definition of the motor number somewhere thats visible to this file
    {
        return COMM_ID_ERROR;
    } else if (numParams != 6) {
        return COMM_PARAM_ERROR;
    }
    else {
        command = (ML_TendonCommandBase *)(new ML_WritePIDCommand);
        command->motor_ref = &tendons[id];
        command->fn = (CommandExecuteFn)ML_WRitePIDCommand_execute;

        // todo

        return COMM_SUCCESS;
    }
}

tendon_comm_result_t ML_SetZeroAngleCommand_create(
    ML_TendonCommandBase* command,
    TendonControl_data_packet_s* dataPacket,
    TendonController* tendons
)
{
    uint8_t id = dataPacket->data_packet_u.data_packet_s.motorId;

    if (id >= 8) // TODO: Move the definition of the motor number somewhere thats visible to this file
    {
        return COMM_ID_ERROR;
    } else {
        command = (ML_TendonCommandBase *)(new ML_SetZeroAngleCommand);
        command->motor_ref = &tendons[id];
        command->fn = (CommandExecuteFn)ML_SetMaxAngleCommand_execute;

        return COMM_SUCCESS;
    }
}

tendon_comm_result_t ML_SetMaxAngleCommand_create(
    ML_TendonCommandBase* command,
    TendonControl_data_packet_s* dataPacket,
    TendonController* tendons
)
{
    uint8_t id = dataPacket->data_packet_u.data_packet_s.motorId;

    if (id >= 8) // TODO: Move the definition of the motor number somewhere thats visible to this file
    {
        return COMM_ID_ERROR;
    } else {
        command = (ML_TendonCommandBase *)(new ML_SetMaxAngleCommand);
        command->motor_ref = &tendons[id];
        command->fn = (CommandExecuteFn)ML_SetMaxAngleCommand_execute;

        uint8_t angle_h = dataPacket->data_packet_u.data_packet_s.pkt_params[0];
        uint8_t angle_l = dataPacket->data_packet_u.data_packet_s.pkt_params[1];

        ((ML_SetMaxAngleCommand *)command)->angle = (int)TENDON_CONTROL_MAKE_16B_WORD(angle_h, angle_l);

        return COMM_SUCCESS;
    }   
}

tendon_comm_result_t CommandFactory_CreateCommand(
    ML_TendonCommandBase* command,
    TendonControl_data_packet_s* dataPacket,
    TendonController* tendons
)
{
    tendon_comm_result_t result = COMM_SUCCESS;

    switch (dataPacket->data_packet_u.data_packet_s.opcode)
    {
        case ECHO:
            result = ML_EchoCommand_create(command, dataPacket, tendons);
            break;
        case READ_STATUS:
            result = ML_ReadStatusCommand_create(command, dataPacket, tendons);
            break;
        case READ_ANGLE:
            result =  ML_ReadAngleCommand_create(command, dataPacket, tendons);
            break;
        case WRITE_ANGLE:
            result = ML_WriteAngleCommand_create(command, dataPacket, tendons);
            break;
        case WRITE_PID:
            result =  ML_WritePIDCommand_create(command, dataPacket, tendons);
            break;
        case SET_ZERO_ANGLE:
            result =  ML_SetZeroAngleCommand_create(command, dataPacket, tendons);
            break;
        case SET_MAX_ANGLE:
            result = ML_SetMaxAngleCommand_create(command, dataPacket, tendons);
            break;
        default:
            result = COMM_INSTRUCTION_ERROR;
    }

    return result;
}