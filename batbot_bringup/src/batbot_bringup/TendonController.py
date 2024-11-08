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
    '''
    This class is used to control and interface with NEEC motor controller via a
    serial connection. The relevant firmware for the motor controller is located
    in the batbot_tendon_controller folder.

    HOW TO USE
    ==========
    First, create a TendonController object specifying the communication
    type and port number. Example:

    `tendonController = TendonController(port_name="/dev/ttyACM0")`

    You can then call any function to control a motor. Example:

    `tendonController.writeMotorAngle(0, 120)`
    
    '''


    def __init__(self, com=COM_TYPE.NONE, port_name=''):
        '''
        Creates a tendon controller object.

        The com argument specifies the serial interface that
        the motor controller is connected by (SPI or UART).

        The port name gives the serial port or device on which
        the motor controller is connected (e.g. COM9 or /dev/ttyACM0)
        '''
        self.th = TendonHardwareInterface(port_name)
            

    def connectToDev(self, com:COM_TYPE, port_name):
        print()

        raise NotImplementedError

    def writeMotorAngle(self, id, angle):
        '''
        This function sets the motor specified by id to move to angle.
        The angle should be a signed integer between -360 to 360.
        '''
    
        # angle_h = (angle >> 8) & 0xFF
        # angle_l = angle & 0xFF

        # params = [angle_h, angle_l]

        # seq = ctypes.c_uint8 * len(params)
        # arr = seq(*params)

        # lib.BuildPacket(self.TendonInterface, id, OPCODE.WRITE_ANGLE.value, arr, len(params))
        # lib.SendTx(self.TendonInterface)

        raise NotImplementedError

    def readMotorAngle(self, id):
        '''
        This function returns the angle of the motor specified by id.
        '''

        # params = []

        # seq = ctypes.c_uint8 * len(params)
        # arr = seq(*params)

        # lib.BuildPacket(self.TendonInterface, id, OPCODE.READ_ANGLE.value, arr, len(params))
        # lib.SendTxRx(self.TendonInterface)

        raise NotImplementedError

    def moveMotorToMin(self, id):
        '''
        This function moves the motor specified by id to its minimum angle
        '''
        raise NotImplementedError

    def moveMotorToMax(self, id):
        '''
        This function moves the motor specified by id to its maximum angle
        '''
        raise NotImplementedError

    def moveMotorToZero(self, id):
        '''
        This function moves the motor specified by id to 0
        '''
        raise NotImplementedError

    def setNewZero(self, id):
        '''
        For the motor specified by id, sets its current angle to the zero angle
        '''
        raise NotImplementedError

if __name__ == "__main__":

    import time

    tc = TendonController(port_name="/dev/ttyACM0")

    while True:
        tc.writeMotorAngle(0, 120)
        time.sleep(0.05)
        tc.writeMotorAngle(0, 0)
        time.sleep(0.05)