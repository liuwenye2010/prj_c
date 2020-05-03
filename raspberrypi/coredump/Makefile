




CROSS_COMPILE ?= /tmp/hdc_toolchains/gcc-linaro-4.9.4-2017.01-i686-mingw32_arm-linux-gnueabihf/bin/arm-linux-gnueabihf-
TARGET_PATH ?=.


$(info CROSS_COMPILE == $(CROSS_COMPILE)) 


CC                      =  $(CROSS_COMPILE)g++
AR                      =  $(CROSS_COMPILE)ar
OBJECTS                 := $(SRCS_CPP:.cpp=.o)
#CPPFLAGS                += -x c++

TEST_SRCS=\
	helloworld.cpp


TEST_OBJS=$(TEST_SRCS:.cpp=.o)
TEST_TARGETS :=$(TEST_SRCS:.cpp=.test)
TEST_LIBS=$(TARGET_PATH)/helloworld.a

.PHONY: all clean  $(TEST_LIBS) $(TEST_TARGETS)  $(TEST_OBJS)

all: $(TEST_TARGETS)



$(TEST_TARGETS):$(TEST_SRCS)
	$(CC) $(CPPFLAGS) $< -o $@

#%.o:%.cpp
$(TEST_OBJS):$(TEST_SRCS)
	$(CPP) $(CPPFLAGS) -c $<

#$(TEST_LIBS):*.o	
$(TEST_LIBS):$(TEST_OBJS)
	$(AR) $(ARFLAGS) $@  $<

clean:
	rm -f $(TEST_TARGETS) $(TEST_OBJS) $(TEST_LIBS)