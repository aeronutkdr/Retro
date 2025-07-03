OBJ_DIR   = Object
CC        = g++
CFLAGS    = -Wall -c -g -O0
MODULE    = ev
OBJECTS   = $(OBJ_DIR)/$(MODULE).o\
	        $(OBJ_DIR)/evFile.o\
	        $(OBJ_DIR)/evRecordCom.o\
	        $(OBJ_DIR)/evRecordData.o\
	        $(OBJ_DIR)/evRecordId.o\
	        $(OBJ_DIR)/evRecordInfo.o\
	        $(OBJ_DIR)/evRecordPlay.o\
	        $(OBJ_DIR)/evRecordRadj.o\
	        $(OBJ_DIR)/evRecordStart.o\
	        $(OBJ_DIR)/evRecordSub.o\
	        $(OBJ_DIR)/evRecordVersion.o\
	        $(OBJ_DIR)/evWeights.o\
	        $(OBJ_DIR)/evGame.o

$(OBJ_DIR)/$(MODULE).exe : $(OBJ_DIR) $(OBJECTS)
	$(CC) -o $@ $(OBJECTS)

$(OBJ_DIR) :
	if not exist $(OBJ_DIR) mkdir $(OBJ_DIR)

$(OBJ_DIR)/%.o : %.cpp %.h
	$(CC) $(CFLAGS) $< -o $@

$(OBJ_DIR)/%.o : %.cpp
	$(CC) $(CFLAGS) $< -o $@

####################
# Build Rule - Clean
####################
.PHONY: clean
clean:
	rmdir /q /s $(OBJ_DIR)
