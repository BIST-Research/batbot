#ifndef ML_TENDON_COMMANDS_H
#define ML_TENDON_COMMANDS_H

#include "ml_tendon_comm_protocol.hpp"
#include <TendonMotor.h>

struct ML_TendonCommandBase;

typedef struct CommandReturn_t {
    size_t numParams;
    uint8_t* params;
} CommandReturn_t;

typedef CommandReturn_t (*CommandExecuteFn)(struct ML_TendonCommandBase * self);

typedef struct ML_TendonCommandBase {
    CommandExecuteFn fn;
    TendonController* motor_ref;
} ML_TendonCommandBase;

typedef struct ML_EchoCommand {
    ML_TendonCommandBase base;
    size_t numParams;
    uint8_t* params;
} ML_EchoCommand;

typedef struct ML_ReadStatusCommand {
    ML_TendonCommandBase base;
} ML_ReadStatusCommand;

typedef struct ML_ReadAngleCommand {
    ML_TendonCommandBase base;
} ML_ReadAngleCommand;

typedef struct ML_WriteAngleCommand {
    ML_TendonCommandBase base;
    int anglePercent;
} ML_WriteAngleCommand;

typedef struct ML_WritePIDCommand {
    ML_TendonCommandBase base;
    float P;
    float I;
    float D;
} ML_WritePIDCommand;

typedef struct ML_SetZeroAngleCommand {
    ML_TendonCommandBase base;
} ML_SetZeroAngleCommand;

typedef struct ML_SetMaxAngleCommand {
    ML_TendonCommandBase base;
    int angle;
} ML_SetMaxAngleCommand;

tendon_comm_result_t CommandFactory_CreateCommand(
    ML_TendonCommandBase* command,
    TendonControl_data_packet_s* dataPacket,
    TendonController* tendons
);

#endif