OBJ_DIR   = Object
CC        = gcc
CFLAGS    = -Wall -c -g -O0 -DDEBUG
MODULE    = evAllocate
OBJECTS   = $(OBJ_DIR)/$(MODULE).o

$(OBJ_DIR)/$(MODULE).exe : $(OBJ_DIR) $(OBJECTS)
	$(CC) -o $@ $(OBJECTS)

$(OBJ_DIR) :
	if not exist $(OBJ_DIR) mkdir $(OBJ_DIR)

$(OBJ_DIR)/%.o : %.c
	$(CC) $(CFLAGS) $< -o $@

####################
# Build Rule - Clean
####################
.PHONY: clean
clean:
	rmdir /q /s $(OBJ_DIR)
