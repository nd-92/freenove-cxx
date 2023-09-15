# Makefile:
# libWS281X
#################################################################################

DESTDIR = /usr
PREFIX = /local

CXX = g++
CXXSTANDARD = -std=c++14
OPTFLAGS = -O3 -flto
MFLAGS = -mcpu=native
PFLAGS = -pthread
WFLAGS = -D_FORTIFY_SOURCE=2 -Werror -Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion -Wformat-security -Winline -Wformat=2 -Wattributes \
-Wbuiltin-macro-redefined -Wcast-align -Wdiv-by-zero -Wdouble-promotion -Wfloat-equal -Wint-to-pointer-cast -Wlogical-op -Woverflow \
-Wpointer-arith -Wredundant-decls -Wshadow -Wsign-promo -Wwrite-strings -Wimplicit-fallthrough=5 -Wstringop-overflow=4 -Wstrict-aliasing=3 \
-Wdate-time
EXTRA_CXXFLAGS = -pipe -fPIC -DNDEBUG -g -fwrapv -fstack-protector-strong

INCLUDE	= -I. -I/usr/include/python3.7m
CXXFLAGS = $(CXXSTANDARD) $(OPTFLAGS) $(MFLAGS) $(PFLAGS) $(WFLAGS) $(EXTRA_CXXFLAGS) $(INCLUDE)

LIBS = -lm -lpthread -lrt -lcrypt

###############################################################################

default:
	$(CXX) $(CXXFLAGS) -c dma.c -o dma.o
	$(CXX) $(CXXFLAGS) -c mailbox.c -o mailbox.o
	$(CXX) $(CXXFLAGS) -c pcm.c -o pcm.o
	$(CXX) $(CXXFLAGS) -c pwm.c -o pwm.o
	$(CXX) $(CXXFLAGS) -c rpihw.c -o rpihw.o
	$(CXX) $(CXXFLAGS) -c ws2811.c -o ws2811.o
	$(CXX) $(CXXFLAGS) -c Freenove_WS2812_Lib_for_Raspberry_Pi.cpp -o Freenove_WS2812_Lib_for_Raspberry_Pi.o
	$(CXX) $(CXXFLAGS) -shared -Wl,-O1 -Wl,-Bsymbolic-functions -Wl,-z,relro dma.o mailbox.o pcm.o pwm.o rpihw.o ws2811.o Freenove_WS2812_Lib_for_Raspberry_Pi.o -o libWS281X.so

install:
	$(CXX) $(CXXFLAGS) -c dma.c -o dma.o
	$(CXX) $(CXXFLAGS) -c mailbox.c -o mailbox.o
	$(CXX) $(CXXFLAGS) -c pcm.c -o pcm.o
	$(CXX) $(CXXFLAGS) -c pwm.c -o pwm.o
	$(CXX) $(CXXFLAGS) -c rpihw.c -o rpihw.o
	$(CXX) $(CXXFLAGS) -c ws2811.c -o ws2811.o
	$(CXX) $(CXXFLAGS) -c Freenove_WS2812_Lib_for_Raspberry_Pi.cpp -o Freenove_WS2812_Lib_for_Raspberry_Pi.o
	$(CXX) $(CXXFLAGS) -shared -Wl,-O1 -Wl,-Bsymbolic-functions -Wl,-z,relro dma.o mailbox.o pcm.o pwm.o rpihw.o ws2811.o Freenove_WS2812_Lib_for_Raspberry_Pi.o -o libWS281X.so
	@ sudo cp clk.h dma.h gpio.h mailbox.h pcm.h pwm.h rpihw.h ws2811.h Freenove_WS2812_Lib_for_Raspberry_Pi.hpp $(DESTDIR)$(PREFIX)/include/
	@ sudo cp libWS281X.so $(DESTDIR)$(PREFIX)/lib/
	@ sudo ldconfig

clean:
	@ rm -rf *.so
	@ rm -rf *.o

uninstall:
	@ sudo rm $(DESTDIR)$(PREFIX)/include/clk.h
	@ sudo rm $(DESTDIR)$(PREFIX)/include/dma.h
	@ sudo rm $(DESTDIR)$(PREFIX)/include/gpio.h
	@ sudo rm $(DESTDIR)$(PREFIX)/include/mailbox.h
	@ sudo rm $(DESTDIR)$(PREFIX)/include/pcm.h
	@ sudo rm $(DESTDIR)$(PREFIX)/include/pwm.h
	@ sudo rm $(DESTDIR)$(PREFIX)/include/rpihw.h
	@ sudo rm $(DESTDIR)$(PREFIX)/include/ws2811.h
	@ sudo rm $(DESTDIR)$(PREFIX)/include/Freenove_WS2812_Lib_for_Raspberry_Pi.hpp
	@ sudo rm $(DESTDIR)$(PREFIX)/lib/libWS281X.so
	@ sudo ldconfig