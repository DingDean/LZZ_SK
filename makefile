CC=g++

IDIR=include
TEST_DIR=test
SRC=src
ODIR=obj
BIN=bin

CFLAGS=-I $(IDIR) -std=c++11

TEST_OBJ=$(patsubst $(TEST_DIR)/%.cc, $(ODIR)/%.t, $(wildcard $(TEST_DIR)/*.cc))
PROD_OBJ=$(patsubst $(SRC)/%.cc, $(ODIR)/%.o, $(wildcard $(SRC)/*.cc))
OBJ=$(PROD_OBJ)$(TEST_OBJ)

$(ODIR)/%.t:$(TEST_DIR)/%.cc
	$(CC) -c -o $@ $< $(CFLAGS)

$(ODIR)/%.o:$(SRC)/%.cc
	$(CC) -c -o $@ $< $(CFLAGS)

.PHONY:test

test:$(OBJ)
	$(CC) -o $(BIN)/$@ $^ $(CFLAGS) && ./$(BIN)/$@

.PHONY:clean

clean:
	rm -rf $(OBJ)
    