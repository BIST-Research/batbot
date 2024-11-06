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
            

    def connectDev(self, com:COM_TYPE, port_name):
        print()

    def writeAngle(self, id, angle):
        print("Hello")
        # angle_h = (angle >> 8) & 0xFF
        # angle_l = angle & 0xFF

        # params = [angle_h, angle_l]

        # seq = ctypes.c_uint8 * len(params)
        # arr = seq(*params)

        # lib.BuildPacket(self.TendonInterface, id, OPCODE.WRITE_ANGLE.value, arr, len(params))
        # lib.SendTx(self.TendonInterface)

    def readAngle(self, id):
        print("Hello")
        # params = []

        # seq = ctypes.c_uint8 * len(params)
        # arr = seq(*params)

        # lib.BuildPacket(self.TendonInterface, id, OPCODE.READ_ANGLE.value, arr, len(params))
        # lib.SendTxRx(self.TendonInterface)

    def moveMotorToMin(self):
        print()

    def moveMotorToMax(self):
        print()

    def moveMotorToZero(self):
        print()

    def setNewZero(self):
        print()

if __name__ == "__main__":
    tc = TendonController(port_name="/dev/ttyACM0")

    # while True:
    #     tc.writeAngle(0, 120)
    #     time.sleep(0.05)
    #     tc.writeAngle(0, 0)
    #     time.sleep(0.05)
    #     # tc.readAngle(0)

    # tc.writeAngle(0, 0)