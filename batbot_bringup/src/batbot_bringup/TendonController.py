# from tendonhardware import TendonHardwareInterface
from TendonHardware import TendonHardwareInterface

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

        raise NotImplementedError

    def writeMotorAngle(self, id, angle):
        '''
        This function sets the motor specified by id to move to angle.
        The angle should be a signed integer between -360 to 360.
        '''
    
        angle_h = (angle >> 8) & 0xFF
        angle_l = angle & 0xFF

        params = [angle_h, angle_l]

        self.th.BuildPacket(id, OPCODE.WRITE_ANGLE.value, params)
        self.th.SendTx()

    def readMotorAngle(self, id):
        '''
        This function returns the angle of the motor specified by id.
        '''

        self.th.BuildPacket(id, OPCODE.READ_ANGLE.value, [])
        ret = self.th.SendTxRx()

        angle = (ret["params"][0] << 8) | (ret["params"][1] & 0xFF)

        return angle

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
        tc.writeMotorAngle(0, 0)
        print("Writing angle...")
        time.sleep(1)
        angle = tc.readMotorAngle(0)
        print("Received angle", angle)