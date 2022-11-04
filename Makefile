CXXFLAGS=-Wall -O3 -g
OBJECTS=matrix-udpserver.o
BINARIES=matrix-udpserver

RGB_INCDIR=matrix/include
RGB_LIBDIR=matrix/lib
RGB_LIBRARY_NAME=rgbmatrix
RGB_LIBRARY=$(RGB_LIBDIR)/lib$(RGB_LIBRARY_NAME).a

ASIO_H_DIR = asio/asio/include'

LDFLAGS+= -L$(RGB_LIBDIR) -l$(RGB_LIBRARY_NAME) -L$(ASIO_H_DIR)\
          -lrt -lm -lpthread

all : matrix-udpserver

matrix-udpserver : $(OBJECTS) $(RGB_LIBRARY)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $@ $(LDFLAGS)

$(RGB_LIBRARY): FORCE
	$(MAKE) -C $(RGB_LIBDIR)

matrix-udpserver.o : matrix-udpserver.cc

%.o : %.cc
	$(CXX) -I$(RGB_INCDIR) -I$(ASIO_H_DIR) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJECTS) $(BINARIES)
	$(MAKE) -C $(RGB_LIBDIR) clean

FORCE:
.PHONY: FORCE