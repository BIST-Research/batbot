from tendonhardware import TendonHardwareInterface

from enum import Enum

class COM_TYPE(Enum):
    NONE = -1
    SPI = 0
    FAKE_SPI = 1
    UART = 2

class OPCODE(Enum):
    ECHO = 0
    READ_STATUS = 1
    READ_ANGLE = 2
    WRITE_ANGLE = 3
    WRITE_PID = 4


class TendonController:
    def __init__(self, com=COM_TYPE.NONE, port_name=''):
        self.th = TendonHardwareInterface(port_name)
            

    def connectToDev(self, com:COM_TYPE, port_name):
        print()

        raise NotImplementedError

    def writeMotorAngle(self, id, angle):
        # angle_h = (angle >> 8) & 0xFF
        # angle_l = angle & 0xFF

        # params = [angle_h, angle_l]

        # seq = ctypes.c_uint8 * len(params)
        # arr = seq(*params)

        # lib.BuildPacket(self.TendonInterface, id, OPCODE.WRITE_ANGLE.value, arr, len(params))
        # lib.SendTx(self.TendonInterface)

        raise NotImplementedError

    def readMotorAngle(self, id):
        # params = []

        # seq = ctypes.c_uint8 * len(params)
        # arr = seq(*params)

        # lib.BuildPacket(self.TendonInterface, id, OPCODE.READ_ANGLE.value, arr, len(params))
        # lib.SendTxRx(self.TendonInterface)

        raise NotImplementedError

    def moveMotorToMin(self):
        raise NotImplementedError

    def moveMotorToMax(self):
        raise NotImplementedError

    def moveMotorToZero(self):
        raise NotImplementedError

    def setNewZero(self):
        raise NotImplementedError

if __name__ == "__main__":
    import time

    tc = TendonController(port_name="/dev/ttyACM0")

    while True:
        tc.writeAngle(0, 120)
        time.sleep(0.05)
        tc.writeAngle(0, 0)
        time.sleep(0.05)
        # tc.readAngle(0)