from ..bb_tendons.TendonHardware import TendonHardwareInterface
import time
import numpy as np

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

        # The following variables are used if no device is connected
        self.test_mode = True
        self.test__angle = 0
        self.test__max_angle = 0

        print(port_name)

        if port_name != '':
            self.th = TendonHardwareInterface(port_name)
            self.test_mode = False
        else:
            print("WARNING: Beginning tendon calibration in test mode! Please supply a port name if this wasn't intentional.")
            time.sleep(3)

    def writeMotorAbsoluteAngle(self, id, angle: np.int16):
        '''
        This function sets the motor specified by id to move to the angle
        that is percent of the maximum angle.

        The percent argument is an integer from 0-100 that indicates
        a percentage of the maximum angle, previously set by setMotorMaxAngle,
        to move the motor to.
        '''
        
        if not self.test_mode:
            angle_h = (angle >> 8) & 0xFF
            angle_l = (angle & 0xFF)

            params = [angle_h, angle_l]

            self.th.BuildPacket(id, OPCODE.WRITE_ANGLE.value, params)
            ret = self.th.SendTxRx()

            assert(ret["status"] == 0)
        else:
            self.test__angle = angle

    def readMotorAngle(self, id):
        '''
        This function returns the angle of the motor specified by id.
        '''

        if not self.test_mode:
            self.th.BuildPacket(id, OPCODE.READ_ANGLE.value, [])
            ret = self.th.SendTxRx()

            if (self.test_mode):
                return 0

            if ret != -1:
                assert(ret["status"] == 0)

                angle = np.int16(ret["params"][0] << 8) | (ret["params"][1] & 0xFF)
                return angle
        else:
            return self.test__angle

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
        if not self.test_mode:
            params = []
            
            self.th.BuildPacket(id, OPCODE.SET_ZERO_ANGLE.value, params)
            ret = self.th.SendTxRx()

            assert(ret["status"] == 0)
        else:
            self.test__angle = 0
    
    def setMotorMaxAngle(self, id, angle):
        if not self.test_mode:
            angle_h = (angle >> 8) & 0xFF
            angle_l = angle & 0xFF

            params = [angle_h, angle_l]

            self.th.BuildPacket(id, OPCODE.SET_MAX_ANGLE.value, params)
            ret = self.th.SendTxRx()

            assert(ret["status"] == 0)
        else:
            self.test__max_angle = angle

if __name__ == "__main__":  

    import time

    tc = TendonController(port_name="COM3")

    # tc.setMotorMaxAngle(0, 180)

    angles = [0, 0, 0, 0, 0]

    while True:
        for i in range(0, 5):
            angles[i] = tc.readMotorAngle(i)

        print(angles)

        time.sleep(3)
            