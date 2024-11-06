#include <pybind11/pybind11.h>
#include <tendon_hardware_interface.hpp>


#include <iostream>

namespace py = pybind11;


PYBIND11_MODULE(handle, m) {
    py::class_<TendonHardwareInterface>(m, "TendonHardware")
        .def(py::init<std::string>())
        .def("BuildPacket", &TendonHardwareInterface::BuildPacket)
        .def("SendTxRx", &TendonHardwareInterface::SendTxRx)
        .def("SendTx", &TendonHardwareInterface::SendTx);
}