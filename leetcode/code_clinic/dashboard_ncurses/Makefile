CC		= C:/mingw-w64/mingw64/bin/gcc
TARGET1	= dashboard
SRC1	= dashboard.c
TARGET2	= dashboard_ncurses
SRC2    = dashboard_ncurses.c  
CFLAGS := -I C:/mingw-w64/mingw64/opt/include/ncursesw
CFALGS += -I C:/mingw-w64/mingw64/opt/include
LFLAGS  = -L C:/mingw-w64/mingw64/opt/lib

$(info $(CFALGS))

all: $(TARGET1) $(TARGET2)
$(TARGET1): $(SRC1)
	$(CC) -o $@ $^

$(TARGET2): $(SRC2)
	$(CC) -o $@ $(CFLAGS) $^ $(LFLAGS) -lncurses

$(TARGET): $(SRC)
	$(CC) -o $@ $(CFLAGS) $^

.PHONY: all clean
clean:
	rm -f $(TARGET1) $(TARGET2)