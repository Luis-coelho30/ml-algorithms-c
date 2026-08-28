CC = gcc
CFLAGS = -Wall -Wextra -Iinclude -lm
SRC = src/main.c \
      src/dsa/vector.c \
      src/dsa/matrix.c \
      src/models/linear_model.c \
      src/models/knn_model.c \
      src/data_utils.c \
	  src/utils.c
OUT = bin/ml_benchmark

all:
	$(CC) $(CFLAGS) $(SRC) -o $(OUT)

clean:
	rm -f $(OUT)