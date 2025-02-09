#include <cstring>
#include <string.h>
#include <iostream>

#include "serial_object_uart_linux.hpp"

#ifdef __linux__

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>

SerialObject_UART_Linux::SerialObject_UART_Linux(std::string portName)
{
  _portName = portName;

  _fd = open(_portName.c_str(), O_RDWR | O_NOCTTY | O_SYNC);
  if (_fd < 0) {
    std::cout << "Error opening " << portName << ": "<< strerror(errno) << "\n";
    // throw exception
  } else {
    std::cout << "Successfully opened " << portName << "\n";
    // throw exception
  }
}

SerialObject_UART_Linux::~SerialObject_UART_Linux()
{
  closePort(); 
}

int SerialObject_UART_Linux::set_attributes(int speed, int parity) {
  struct termios tty;
  if (tcgetattr(_fd, &tty) < 0) {
    std::cout << "Error" << errno << " from tcgettatr\n";
    return -1;
  }

  cfsetospeed(&tty, speed);
  cfsetispeed(&tty, speed);

  tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8; // 8-bit characters
  tty.c_iflag &= ~IGNBRK;                     // disable break processing
  tty.c_lflag = 0; // no signaling chars, no echo, no canonical processing

  tty.c_oflag = 0;     // no remapping, no delays
  tty.c_cc[VMIN] = 1;  // read doesn't block
  tty.c_cc[VTIME] = 5; // 0.5 seconds read timeout

  tty.c_iflag &= ~(IXON | IXOFF | IXANY); // shut off xon/xoff ctrl
  tty.c_cflag |= PARENB;
  tty.c_cflag |= (CLOCAL | CREAD);
  tty.c_cflag |= parity;
  tty.c_cflag &= ~CSTOPB;
  tty.c_cflag &= ~CRTSCTS;

  if (tcsetattr(_fd, TCSANOW, &tty) != 0) {
    std::cout << "Error " << errno << " from tcsetattr\n";
    return -1;
  }
  return 0;
}

void SerialObject_UART_Linux::enable_blocking(bool should_block) {
  struct termios tty;
  memset(&tty, 0, sizeof tty);
  if (tcgetattr(_fd, &tty) != 0) {
    std::cout << "Error" << errno << " from tcgettatr\n";
    return;
  }

  tty.c_cc[VMIN] = should_block ? 1 : 0;
  tty.c_cc[VTIME] = 5; // 0.5 seconds read timeout

  if (tcsetattr(_fd, TCSANOW, &tty) != 0) {
    std::cout << "Error " << errno << " from tcsetattr\n";
  }
}

void SerialObject_UART_Linux::writeBytes(const uint8_t* bytes, int numBytes) {
  write(_fd, bytes, numBytes);
}

int SerialObject_UART_Linux::readBytes(uint8_t * buff, int numBytes) {
  return read(_fd, buff, numBytes);
}

void SerialObject_UART_Linux::closePort()
{
  close(_fd);
  std::cout << "Closed port " << _portName << "\n";
}

#endif