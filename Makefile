BIN = terminate \
      straight \
      course_m \
      course_n \
      course_q \
      course_t \
      course_t_setter \

ifeq ($(shell ls | grep pigpiod_if2_exists), pigpiod_if2_exists)
 PIFLAG = -lpigpiod_if2 -D IN_RASPBERRYPI
else
# PIFLAG = -lpigpiod_if2 -D IN_RASPBERRYPI
 PIFLAG =
endif

all: $(BIN)

clean:
	rm -f $(BIN) *.o *.swp

exet:
	./course_t

exets:
	./course_t_setter

terminate: terminate.o
	gcc -Wall -pthread -o $@ $^ $(PIFLAG) -lrt

straight: straight.o
	gcc -Wall -pthread -o $@ $^ $(PIFLAG) -lrt

course_m: course_m.o
	gcc -Wall -pthread -o $@ $^ $(PIFLAG) -lrt

course_n: course_n.o
	gcc -Wall -pthread -o $@ $^ $(PIFLAG) -lrt

course_q: course_q.o
	gcc -Wall -pthread -o $@ $^ $(PIFLAG) -lrt

course_t: course_t.c
	gcc -Wall -pthread -o $@ $^ $(PIFLAG) -lrt

course_t_setter: course_t_setter.o
	gcc -Wall -pthread -o $@ $^ $(PIFLAG) -lrt
