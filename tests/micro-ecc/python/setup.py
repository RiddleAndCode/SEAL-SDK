#!/usr/bin/env python

from distutils.core import setup
import os


def read(fname):
    return open(os.path.join(os.path.dirname(__file__), fname)).read()


setup(
    name='microecc-py',
    version='1.0.0',
    description='Python wrapper around microECC C-library',
    long_description=read('README.md'),
    author='León Domingo',
    author_email='leon@riddleandcode.com',
    url='https://www.riddleandcode.com',
    packages=['microecc_py'],
)
