#include "serial_object.hpp"

#include <iostream>
#include "string"
#include "serial_object_uart_win.hpp"

int main(int argc, char* argv[])
{
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << "<port>\n";
        return -1;
    }

    std::string portName = argv[1];
    
    SerialObject *ser = new SerialObject_UART_Win(portName);

    // ((SerialObject_UART_Linux *)ser)->set_attributes(115200, 1);


    // uint8_t data[] = {0xFF, 0x00, 0x02, 0x00, 0x02};
    // int len = 5;

    // ser->writeBytes(data, len);

    // uint8_t buff[32];

    // ser->readBytes(buff, 32);

    // for (int i = 0; i < 32; ++i)
    // {
    //     std::cout << buff[i] << " ";
    // }
    // std::cout << "\n";
}