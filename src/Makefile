LIBS += -ldl -llttng-ust -lpthread -lpopt
CFLAGS = -I../utils
BIN = ustbench
BINDIR = bin
LIBS = -ldl -llttng-ust -lpthread -lpopt -lurcu-bp

all: libbench.so
#all: $(BIN)

debug: CFLAGS += -g
debug: all

install:
	install libbench.so /usr/local/lib64/
	install libustperf.h /usr/local/include
	install utils.h /usr/local/include

libbench.so: libbench.c
	gcc $(CFLAGS) -shared -fPIC -o libbench.so libbench.c

$(BINDIR):
	mkdir -p $(BINDIR)

clean:
	rm *.so
	rm -rf $(BINDIR)
