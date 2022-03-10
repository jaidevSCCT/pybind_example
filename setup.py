from setuptools import setup, Extension
from setuptools.command.build_ext import build_ext
import pybind11

"""
Example has been adapted from https://github.com/pybind/python_example
"""

ext_modules = [
    Extension(
        "pybind_tutorial",
        define_macros=[],
        sources=["src/pybind_tutorial.cpp"],
        include_dirs=["pybind11.get_include(user=True)", "src/"],
        library_dirs=[],
        libraries=[],
        language="c++"
    ),
]

__version__ = "1.0"

setup(
    name="pybind_tutorial",
    version=__version__,
    author="jaidev",
    description="pybind11 tutorial",
    ext_modules=ext_modules,
    install_requires=["pybind11", "setuptools>=42", "wheel", "numpy"],
    cmdclass={"build_ext": build_ext},
    zip_safe=False,
    python_requires=">=3.6",
)

