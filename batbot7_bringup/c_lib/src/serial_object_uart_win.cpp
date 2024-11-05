

#include <cstring>
#include <string.h>
#include <iostream>

#include "serial_object_uart_win.hpp"
// #include <windows.h>

SerialObject_UART_Win::SerialObject_UART_Win(std::string portName)
{
  // HANDLE port = CreateFileA(portName.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL,
  //   OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
  // if (port == INVALID_HANDLE_VALUE)
  // {

    
  //   std::cout << "error in SerialObject UART_win"; 
  // }
 
  // // Flush away any bytes previously read or written.
  // BOOL success = FlushFileBuffers(port);
  // if (!success)
  // {
  //   std::cout<<"Failed to flush serial port";
  //   CloseHandle(port);
    
  // }

}

SerialObject_UART_Win::~SerialObject_UART_Win()
{
  closePort();
}

int SerialObject_UART_Win::set_attributes(int speed, int parity) {
  // Set the baud rate and other options.
  // DCB state = {0};
  // state.DCBlength = sizeof(DCB);
  // state.BaudRate = speed;
  // state.ByteSize = 8;
  // state.Parity = parity;
  // state.StopBits = ONESTOPBIT;
  // BOOL success = SetCommState(port, &state);
  // if (!success)
  // {
  //   std::cout << "Failed to set serial settings";
  //   CloseHandle(port);
  //   return -1;
    
  // }
  return 1;
}

void SerialObject_UART_Win::enable_blocking(bool should_block) {
  // Configure read and write operations to time out after 100 ms.
  // COMMTIMEOUTS timeouts = {0};
  // timeouts.ReadIntervalTimeout = 0;
  // timeouts.ReadTotalTimeoutConstant = (should_block) ? 100 : 0;
  // timeouts.ReadTotalTimeoutMultiplier = 0;
  // timeouts.WriteTotalTimeoutConstant = (should_block) ? 100 : 0;
  // timeouts.WriteTotalTimeoutMultiplier = 0;

  // BOOL success = SetCommTimeouts(port, &timeouts);
  // if (!success)
  // {
  //   std::cout<<"Failed to set serial timeouts";
  //   CloseHandle(port);
  // }
}

void SerialObject_UART_Win::writeBytes(const uint8_t* bytes, int numBytes) {
  // DWORD written;
  // BOOL success = WriteFile(port, bytes, numBytes, &written, NULL);
  // if (!success)
  // {
  //   std::cout<<"Failed to write to port";
    
  // }
  // if (written != numBytes)
  // {
  //   std::cout<< "Failed to write all bytes to port";
    
  // }
  
}

int SerialObject_UART_Win::readBytes(uint8_t * buff, int numBytes) {
  // DWORD received;
  // BOOL success = ReadFile(port, buff, numBytes, &received, NULL);
  // if (!success)
  // {
  //   std::cout<<"Failed to read from port";
  //   return -1;
  // }
  // return received;
  
}

void SerialObject_UART_Win::closePort() {
  // CloseHandle(port); 
}

