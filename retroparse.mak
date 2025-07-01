OBJ_DIR   = Object
CC        = gcc
CFLAGS    = -Wall -c -g -O0
MODULE    = retroparse
OBJECTS   = $(OBJ_DIR)/$(MODULE).o\
            $(OBJ_DIR)/$(MODULE)_test.o\
            $(OBJ_DIR)/NodeTree.o\
            $(OBJ_DIR)/GJ.o

$(OBJ_DIR)/$(MODULE)_test.exe : $(OBJ_DIR) $(OBJECTS)
	$(CC) -o $@ $(OBJECTS)

$(OBJ_DIR) :
	if not exist $(OBJ_DIR) mkdir $(OBJ_DIR)

$(OBJ_DIR)/$(MODULE)_test.o : $(MODULE)_test.c
	$(CC) $(CFLAGS) $< -o $@

#$(OBJ_DIR)/$(MODULE).o : $(MODULE).c
#	$(CC) $(CFLAGS) $< -o $@

$(OBJ_DIR)/%.o : %.c %.h *.h
	$(CC) $(CFLAGS) $< -o $@

####################
# Build Rule - Clean
####################
.PHONY: clean
clean:
	rmdir /q /s $(OBJ_DIR)