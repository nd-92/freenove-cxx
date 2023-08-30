# Makefile:
#	wiringPi - Wiring Compatable library for the Raspberry Pi
#
#	Copyright (c) 2012-2015 Gordon Henderson
#################################################################################
# This file is part of wiringPi:
#	https://github.com/wiringPi/wiringPi
#
#    wiringPi is free software: you can redistribute it and/or modify
#    it under the terms of the GNU Lesser General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    wiringPi is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU Lesser General Public License for more details.
#
#    You should have received a copy of the GNU Lesser General Public License
#    along with wiringPi.  If not, see <http://www.gnu.org/licenses/>.
#################################################################################

VERSION = $(shell cat ../VERSION)
DESTDIR = /usr
PREFIX = /local
LDCONFIG = sudo ldconfig

ifneq ($V,1)
Q = @
endif

DYNAMIC = libwiringPi.so.$(VERSION)

CXX = g++
CXXSTANDARD = -std=c++14
OPTFLAGS = -O3 -flto
MFLAGS = -mcpu=native
PFLAGS = -pthread
WFLAGS = -D_FORTIFY_SOURCE=2 -Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion -Wformat-security -Winline -Wformat=2 -Wattributes \
-Wbuiltin-macro-redefined -Wcast-align -Wdiv-by-zero -Wdouble-promotion -Wfloat-equal -Wint-to-pointer-cast -Wlogical-op -Woverflow \
-Wpointer-arith -Wredundant-decls -Wshadow -Wsign-promo -Wwrite-strings -Wimplicit-fallthrough=5 -Wstringop-overflow=4 -Wstrict-aliasing=3
EXTRA_CXXFLAGS = -pipe -fPIC

INCLUDE	= -I.
CXXFLAGS = $(CXXSTANDARD) $(OPTFLAGS) $(MFLAGS) $(PFLAGS) $(WFLAGS) $(EXTRA_CXXFLAGS) $(INCLUDE)

LIBS = -lm -lpthread -lrt -lcrypt

###############################################################################

default:
	$(CXX) $(CXXFLAGS) -c ADCDevice.C
	$(CXX) $(CXXFLAGS) -shared -o libADCDevice.so ADCDevice.o

install:
	$(CXX) $(CXXFLAGS) -c ADCDevice.C
	$(CXX) $(CXXFLAGS) -shared -o libADCDevice.so ADCDevice.o
	@ sudo cp ADCDevice.H /usr/include/
	@ sudo cp libADCDevice.so /usr/lib/
	@ sudo ldconfig

clean:
	@ rm -rf *.so
	@ rm -rf *.o

uninstall:
	@ sudo rm /usr/include/ADCDevice.H
	@ sudo rm /usr/lib/libADCDevice.so
	@ sudo ldconfig