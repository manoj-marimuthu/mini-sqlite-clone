TARGET = db.exe
COMMAND = gcc -Wall -g -Iinclude

OBJ = build/db.o \
	  build/db_error.o \
	  build/db_memory.o \
	  build/db_lexer.o \
	  build/db_parser.o \
	  build/db_executor.o \
	  build/db_dot_command.o

$(TARGET): $(OBJ)
	$(COMMAND) $(OBJ) -o $(TARGET)
build/db.o : db.c
	$(COMMAND) -c db.c -o build/db.o
build/db_error.o : scripts/db_error.c
	$(COMMAND) -c scripts/db_error.c -o build/db_error.o
build/db_memory.o : scripts/db_memory.c
	$(COMMAND) -c scripts/db_memory.c -o build/db_memory.o
build/db_lexer.o : scripts/db_lexer.c
	$(COMMAND) -c scripts/db_lexer.c -o build/db_lexer.o
build/db_parser.o : scripts/db_parser.c
	$(COMMAND) -c scripts/db_parser.c -o build/db_parser.o
build/db_executor.o : scripts/db_executor.c
	$(COMMAND) -c scripts/db_executor.c -o build/db_executor.o
build/db_dot_command.o : scripts/db_dot_command.c
	$(COMMAND) -c scripts/db_dot_command.c -o build/db_dot_command.o
clean:
	del *.exe
	del build\*.o