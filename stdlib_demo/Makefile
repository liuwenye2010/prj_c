CC		= C:/mingw-w64/mingw64/bin/gcc


TARGET1	= files 
SRC1	= files.c

TARGET1	= files_ch
SRC1	= files_ch.c

all:$(TARGET1) $(TARGET2)
$(TARGET1): $(SRC1)
	$(CC) -o $@ $^
$(TARGET2): $(SRC2)
	$(CC) -o $@ $^

.PHONY: clean
clean:
	rm -f $(TARGET)