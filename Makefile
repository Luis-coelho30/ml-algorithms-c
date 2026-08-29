CC = gcc
CFLAGS = -Wall -Wextra -Iinclude -lm
CFLAGS_PAR = -Wall -Wextra -Iinclude -lm -fopenmp

SRC = src/dsa/vector.c \
      src/dsa/matrix.c \
      src/models/linear_model.c \
      src/models/knn_model.c \
      src/data_utils.c \
      src/utils.c

SRC_PAR = src/dsa/vector_par.c \
          src/dsa/matrix_par.c \
          src/dsa/vector.c \
          src/dsa/matrix.c \
          src/models/linear_model_par.c \
          src/models/linear_model.c \
          src/models/knn_model_par.c \
          src/models/knn_model.c \
          src/data_utils.c \
          src/utils.c

all: seq par

seq:
	mkdir -p bin
	$(CC) $(CFLAGS) src/main.c $(SRC) -o bin/ml_benchmark_seq

par:
	mkdir -p bin
	$(CC) $(CFLAGS_PAR) src/main_par.c $(SRC_PAR) -o bin/ml_benchmark_par

clean:
	rm -f bin/ml_benchmark_seq bin/ml_benchmark_par