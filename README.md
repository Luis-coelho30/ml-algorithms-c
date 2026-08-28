# ml-algorithms-c

A benchmarking project comparing sequential and parallel (OpenMP) implementations of Linear Regression and K-Nearest Neighbors in pure C, applied to the California Housing dataset.

---

## Project Structure

```
ml-algorithms-c/
    src/
        main.c
        data_utils.c
        utils.c
        dsa/
            vector.c
            matrix.c
        models/
            linear_model.c
            knn_model.c
    include/
        vector.h
        matrix.h
        utils.h
        data_utils.h
        linear_model.h
        knn.h
    data/
        train_lr.csv
        test_lr.csv
        train_knn.csv
        test_knn.csv
    bin/
    Makefile
```

---

## Models

### Linear Regression

Trained via gradient descent on 8 housing features to predict median house value. The sequential and parallel versions share the same interface — parallelism is applied over the sample dimension during the forward pass and gradient computation.

### K-Nearest Neighbors (Classification)

Classifies houses into 5 categories based on engineered features such as value-to-income ratio and bedroom density. Uses Quickselect to find the k nearest neighbors in O(n) average time. Parallelism is applied over the outer test-point loop, which is embarrassingly parallel.

#### Housing Classes

```
0 - Luxury         high value, high income, low bedroom ratio, oceanfront
1 - Comfortable    medium-high value, good income, low density
2 - Cost-effective affordable relative to local income, reasonable room count
3 - Working Class  low value, high density, many bedrooms per room
4 - Unaffordable   high value relative to local income
```

---

## Data

The California Housing dataset is used in two forms:

- `train_lr.csv` / `test_lr.csv` — raw 8 features with continuous house price target, used for Linear Regression
- `train_knn.csv` / `test_knn.csv` — same features with class label target (0-4), used for KNN

Class labels are computed in Python via feature engineering before training. The CSV format has no header, with the target value as the last column.

---

## Build

Requires GCC and OpenMP support.

```bash
make
```

Output binary is written to `bin/ml_benchmark`.

To clean:

```bash
make clean
```

---

## Run

```bash
./bin/ml_benchmark
```

Runs sequential and parallel versions of both models, printing predictions and wall-clock timing for each.

---

## Implementation Notes

- Matrix stored as a flat 1D `double*` array with row-major indexing: `data[i * cols + j]`
- Vector and Matrix types manage their own memory and must be freed via `free_vector` and `free_matrix`
- KNN borrows training data pointers — `knn_model_free` does not free the dataset
- Quickselect is implemented in `utils.c` and operates on paired distance/index arrays to avoid allocations inside the prediction loop
- `row_distance` in `matrix.c` computes distances from a single point to all rows of a matrix without heap allocation, reusing a pre-allocated buffer across iterations

---

## Dependencies

- GCC with OpenMP (`-fopenmp`)
- Standard C library (`stdlib.h`, `stdio.h`, `math.h`)
- No external libraries