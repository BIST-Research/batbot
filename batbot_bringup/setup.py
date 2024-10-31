from setuptools import setup, find_packages
from pybind11.setup_helpers import Pybind11Extension, build_ext
import pathlib

here = pathlib.Path(__file__).parent.resolve()

# TODO:
#   - For some reason on Linux, I need to export the c_lib library path
#       or else I get an error that libserial.so can't be found. Need
#       to fix this.
#   - Figure out how to build c_lib with this file so user doesn't have to.
#   - Validate that this build process works on windows
# 

setup(
    name="batbot_bringup",
    version="7.0.0",
    package_dir={"": "src"},
    description="Batbot7 gui bringup application",
    packages=find_packages(where="src"),
    python_requires=">=3.8",
    ext_modules=[
        Pybind11Extension(
            "TendonHardware", 
            ["src/batbot_bringup/TendonHardware.cpp"], 
            include_dirs=["c_lib/include"],
            library_dirs=["c_lib/build/src"],
            libraries=["serial"]
        )
    ]
)