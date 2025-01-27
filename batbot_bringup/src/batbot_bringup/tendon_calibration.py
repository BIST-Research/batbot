import curses
import os

from TendonController import TendonController

NUM_TENDONS = 5
INCREMENT_AMOUNTS = [1, 5, 10]
INCREMENT_IDX = 0

def GetInput(screen, prompt):
    screen.nodelay(False)
    curses.echo()
    screen.addstr(prompt)
    screen.refresh()

    input = screen.getstr()
    # screen.getch()  # Wait for user input

    curses.noecho()
    screen.nodelay(True)
    return input

if __name__ == "__main__":
    tc = TendonController(port_name='COM3')

    screen = curses.initscr()
    screen.keypad(True)
    screen.nodelay(True)
    curses.cbreak()
    curses.noecho()

    screen.addstr(0, 0, "This script is used for manual calibration of the tendon actuators.")
    screen.addstr(2, 0, "For each motor, use the right/left arrow keys to move decrement/increment the motor angle. Once the desired angle is reached press enter to continue calibrating the next motor.")

    screen.addstr(4, 0, "CONTROLS")
    screen.addstr(5, 0, "----------------")
    screen.addstr(6, 0, "left/right arrow keys : increment angle by selected amount (available increments are 1, 5, 10)")
    screen.addstr(7, 0, "up/down arrow keys : increase/decrease angle increment")
    screen.addstr(8, 0, "enter : set the motor's current angle as the zero angle")
    screen.refresh()

    try:

        for i in range(0, NUM_TENDONS):
            screen.move(10, 0)
            screen.clrtobot()
            screen.addstr(f'Calibrating tendon {i + 1}...')
            screen.refresh()

            maxAngleIsValid = False
            maxAngle = None
            while not maxAngleIsValid:
                try:
                    screen.move(11, 1)
                    maxAngle = int(GetInput(screen, f"Enter the maximum angle: "))
                    maxAngleIsValid = True
                except:
                    pass
            screen.move(12, 1)
            screen.addstr(f'Set maximum angle to {maxAngle}')
            tc.setMotorMaxAngle(i, maxAngle)

            goal_angle = 0
            key = None
            screen.move(13, 1)
            screen.addstr(f'Current Motor Goal Angle Percentage Of Max: {goal_angle}           ')
            screen.refresh()
            while not (key == curses.KEY_ENTER or key == 10):

                if key == curses.KEY_LEFT or key == 452:
                    goal_angle -= INCREMENT_AMOUNTS[INCREMENT_IDX]
                    goal_angle = max(0, goal_angle)
                    tc.writeMotorAnglePercentMax(i, max(0, goal_angle))
                elif key == curses.KEY_RIGHT or key == 454:
                    goal_angle += INCREMENT_AMOUNTS[INCREMENT_IDX]
                    goal_angle = min(100, goal_angle)
                    tc.writeMotorAnglePercentMax(i, min(100, goal_angle))
                elif key == curses.KEY_UP or key == 450:
                    INCREMENT_IDX = min(len(INCREMENT_AMOUNTS) - 1, INCREMENT_IDX + 1)
                elif key == curses.KEY_DOWN or key == 456:
                    INCREMENT_IDX = max(0, INCREMENT_IDX - 1)

                angle = tc.readMotorAngle(i)

                screen.move(13, 1)
                screen.addstr(f'Current Motor Goal Angle Percentage Of Max: {goal_angle}           ')
                screen.move(14, 1)
                screen.addstr(f'Current Motor Angle {angle}           ')
                screen.refresh()

                key = screen.getch()

            tc.setNewZero(i)
            
                
    except Exception as e:
        print(e)

    screen.move(16, 0)
    screen.addstr("Finished motor calibration!")    
    screen.refresh()
    curses.napms(1000)
    curses.endwin()
    tc.th.ser.close()