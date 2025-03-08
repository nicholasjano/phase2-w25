CC = gcc
CFLAGS = -Wall -I../include

PARSER_SRC = ../src/parser/parser.c
LEXER_SRC = ../src/lexer/lexer.c
OBJ = parser.o lexer.o

TARGET = parser

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

parser.o: $(PARSER_SRC)
	$(CC) $(CFLAGS) -c -o $@ $<

lexer.o: $(LEXER_SRC)
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: all clean