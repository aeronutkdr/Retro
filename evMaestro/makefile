OBJ_DIR   = Object
CC        = gcc
CFLAGS    = -Wall -c -g -O0
MODULE    = evMaestro
OBJECTS   = $(OBJ_DIR)/$(MODULE).o\
            $(OBJ_DIR)/evBadj.o\
            $(OBJ_DIR)/evData.o\
            $(OBJ_DIR)/evFile.o\
            $(OBJ_DIR)/evGame.o\
            $(OBJ_DIR)/evGameID.o\
            $(OBJ_DIR)/evInfo.o\
            $(OBJ_DIR)/evLadj.o\
            $(OBJ_DIR)/evPlay.o\
            $(OBJ_DIR)/evRRespadj.o\
            $(OBJ_DIR)/evRoster.o\
            $(OBJ_DIR)/evStartSub.o\
            $(OBJ_DIR)/evVersion.o

$(OBJ_DIR)/$(MODULE).exe : $(OBJ_DIR) $(OBJECTS)
	$(CC) -o $@ $(OBJECTS)

$(OBJ_DIR) :
	if not exist $(OBJ_DIR) mkdir $(OBJ_DIR)

$(OBJ_DIR)/$(MODULE).o : $(MODULE).c
	$(CC) $(CFLAGS) $< -o $@

$(OBJ_DIR)/%.o : %.c %.h *.h
	$(CC) $(CFLAGS) $< -o $@

####################
# Build Rule - Clean
####################
.PHONY: clean
clean:
	rmdir /q /s $(OBJ_DIR)