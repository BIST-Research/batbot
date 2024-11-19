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
    SET_ZERO_ANGLE = 5
    SET_MAX_ANGLE = 6


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

    You can then call any function to control a motor. For example,
    to write motor 0 to 120 degrees call:

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

    def writeMotorAnglePercentMax(self, id, percent):
        '''
        This function sets the motor specified by id to move to the angle
        that is percent of the maximum angle.

        The percent argument is an integer from 0-100 that indicates
        a percentage of the maximum angle, previously set by setMotorMaxAngle,
        to move the motor to.
        '''
        
        percent = 0xFF (percent)

        params = [percent]

        self.th.BuildPacket(id, OPCODE.WRITE_ANGLE.value, params)
        ret = self.th.SendTxRx()

        assert(ret["status"] == 0)

    def readMotorAngle(self, id):
        '''
        This function returns the angle of the motor specified by id.
        '''

        self.th.BuildPacket(id, OPCODE.READ_ANGLE.value, [])
        ret = self.th.SendTxRx()

        if ret != -1:
            assert(ret["status"] == 0)

            angle = (ret["params"][0] << 8) | (ret["params"][1] & 0xFF)
            return angle

    def moveMotorToMin(self, id):
        '''
        This function moves the motor specified by id to its minimum angle
        '''
        self.writeMotorAnglePercentMax(id, 0)

    def moveMotorToMax(self, id):
        '''
        This function moves the motor specified by id to its maximum angle
        '''
        self.writeMotorAnglePercentMax(id, 100)

    def setNewZero(self, id):
        '''
        For the motor specified by id, sets its current angle to the zero angle
        '''
        params = []
        
        self.th.BuildPacket(id, OPCODE.SET_ZERO_ANGLE.value, params)
        ret = self.th.SendTxRx()

        assert(ret["status"] == 0)
    
    def setMotorMaxAngle(self, id, angle):
        angle_h = (angle >> 8) & 0xFF
        angle_l = angle & 0xFF

        params = [angle_h, angle_l]

        self.th.BuildPacket(id, OPCODE.SET_MAX_ANGLE.value, params)
        ret = self.th.SendTxRx()

        assert(ret["status"] == 0)


if __name__ == "__main__":  

    import time

    tc = TendonController(port_name="/dev/ttyACM0")

    tc.setMotorMaxAngle(0, 180)

    while True:
        print("Writing angle...")
        tc.writeMotorAnglePercentMax(0, 100)
        time.sleep(0.5)
        print("Reading angle...")
        angle = tc.readMotorAngle(0)
        print("Received angle", angle)