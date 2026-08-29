# ml-algorithms-c

A benchmarking project comparing sequential and parallel (OpenMP) implementations of Linear Regression and K-Nearest Neighbors in pure C, applied to the California Housing dataset.

---

## Project Structure

```
ml-algorithms-c/
    src/
        main.c
        main_par.c
        data_utils.c
        utils.c
        dsa/
            vector.c
            matrix.c
            vector_par.c
            matrix_par.c
        models/
            linear_model.c
            knn_model.c
            linear_model_par.c
            knn_model_par.c
    include/
        vector.h
        matrix.h
        vector_par.h
        matrix_par.h
        utils.h
        data_utils.h
        linear_model.h
        knn_model.h
        linear_model_par.h
        knn_model_par.h
    data/
        housing_lr_train.csv
        housing_lr_test.csv
        housing_knn_train.csv
        housing_knn_test.csv
        train_lr.csv
        test_lr.csv
        train_knn.csv
        test_knn.csv
    notebooks/
        data_extraction.ipynb
    bin/
    Makefile
    README.md
```

### Source

`main.c` — sequential benchmark entry point. Runs Linear Regression and KNN, prints RMSE, accuracy, confusion matrix, and wall-clock timing.

`main_par.c` — parallel benchmark entry point. Runs both models swept across 1, 2, 4, and 8 threads, printing the same metrics plus timing per thread count.

`data_utils.c` — loads a headerless CSV into a `Matrix*` of features and a `Vector*` of targets. The last column is always the target.

`utils.c` — shared algorithmic utilities: Quickselect, RMSE, accuracy, and confusion matrix.

`dsa/vector.c` — sequential Vector implementation: creation, destruction, elementwise operations, dot product, scalar multiply, euclidean distance.

`dsa/matrix.c` — sequential Matrix implementation: creation, destruction, add, subtract, multiply, transpose, matrix-vector multiply, row distance.

`dsa/vector_par.c` — parallel variants of vector operations using OpenMP. Includes `vector.h` — importing `vector_par.h` gives access to both sequential and parallel functions.

`dsa/matrix_par.c` — parallel variants of matrix operations using OpenMP. Includes `matrix.h` — importing `matrix_par.h` gives access to both sequential and parallel functions.

`models/linear_model.c` — sequential Linear Regression: gradient descent training, prediction, memory management.

`models/linear_model_par.c` — parallel Linear Regression: replaces matrix-vector multiply and transpose with parallel variants. Small weight vector operations remain sequential to avoid thread spawn overhead on 8-element arrays.

`models/knn_model.c` — sequential KNN: stores training data by reference, classifies by majority vote among k nearest neighbors using Quickselect.

`models/knn_model_par.c` — parallel KNN: parallelizes the outer test-point loop. Each thread allocates its own distance and index buffers to avoid shared state.

### Data

`housing_lr_train.csv` / `housing_lr_test.csv` — full California Housing dataset, 8 raw features, continuous house price target. Used for Linear Regression.

`housing_knn_train.csv` / `housing_knn_test.csv` — full California Housing dataset, 12 features (8 raw + 4 engineered), class label target (0-4). Used for KNN.

`train_lr.csv` / `test_lr.csv` / `train_knn.csv` / `test_knn.csv` — small handcrafted datasets used for unit testing during development.

### Notebooks

`data_extraction.ipynb` — preprocessing pipeline. Loads the California Housing dataset from `sklearn.datasets`, applies standard scaling, engineers KNN features, assigns class labels, and writes all four CSVs to `data/`.

---

## Models

### Linear Regression

Trained via gradient descent on 8 housing features to predict median house value. The parallel version parallelizes matrix-vector multiplication and transpose over the sample dimension (16k rows). Small vector operations on the 8-element weight vector are kept sequential — thread spawn overhead exceeds the computation cost at that scale.

### K-Nearest Neighbors (Classification)

Classifies houses into 5 categories based on 12 features. Uses Quickselect to find the k nearest neighbors in O(n) average time and majority vote for class assignment. The parallel version parallelizes the outer test-point loop, which is embarrassingly parallel — each test point is classified independently.

#### Housing Classes

Labels are assigned by quantile-based thresholds applied in priority order. The first matching rule wins:

```
Priority 1 — 0 Luxury        high cost-to-income, high income, low bedroom ratio, coastal
Priority 2 — 4 Unaffordable  high cost-to-income, income below top third
Priority 3 — 3 Working Class low value, high occupancy density, high bedroom ratio
Priority 4 — 1 Comfortable   high value, high income, low occupancy density
Priority 5 — 2 Cost-effective default (does not meet any of the above conditions)
```

The priority order matters — Luxury is a strict subset of high cost-to-income houses and must be checked before Unaffordable to prevent misclassification.

---

## Data

The California Housing dataset (20,640 samples, 8 features, no missing values) is sourced from `sklearn.datasets` and preprocessed in `notebooks/data_extraction.ipynb`. Both datasets are split 80/20 into train and test sets with standard scaling (zero mean, unit variance) applied to features before export. Targets are not scaled.

**Linear Regression features** (8 raw):

| Feature | Description |
|---|---|
| MedInc | Median income in the block group (tens of thousands USD) |
| HouseAge | Median house age in years |
| AveRooms | Average rooms per household |
| AveBedrms | Average bedrooms per household |
| Population | Block group population |
| AveOccup | Average persons per household |
| Latitude | Block group latitude |
| Longitude | Block group longitude |

**KNN features** (8 raw + 4 engineered):

| Feature | Formula | Description |
|---|---|---|
| occupancy_density | AveOccup | Persons per household |
| rooms_per_capita | AveRooms / AveOccup | Rooms per person |
| bedroom_ratio | AveBedrms / AveRooms | Fraction of rooms that are bedrooms |
| dist_to_coast | haversine to nearest coastal city | Minimum distance in km to a coastal reference point |

`cost_to_income` (MedHouseVal / MedInc) is computed for label assignment only and excluded from the feature set to avoid target leakage.

---

## Build

Requires GCC with OpenMP support.

```bash
make        # builds both binaries
make seq    # sequential only
make par    # parallel only
make clean  # removes binaries
```

Outputs:

```
bin/ml_benchmark_seq
bin/ml_benchmark_par
```

---

## Run

```bash
./bin/ml_benchmark_seq
./bin/ml_benchmark_par
```

The sequential binary runs both models once and reports timing, RMSE, accuracy, and confusion matrix.

The parallel binary sweeps each model across 1, 2, 4, and 8 threads and reports the same metrics plus wall-clock timing per run.

---

## Implementation Notes

- Matrix stored as a flat 1D `double*` array with row-major indexing: `data[i * cols + j]`
- Vector and Matrix types manage their own memory and must be freed via `free_vector` and `free_matrix`
- KNN borrows training data pointers — `knn_model_free` does not free the dataset
- Quickselect operates on paired distance/index arrays so neighbor indices are tracked through the partition without a separate sort
- `row_distance` computes distances from a single test point to all training rows into a pre-allocated buffer — no heap allocation inside the classification loop
- In the parallel KNN implementation, each thread allocates its own distance and index buffers to avoid race conditions
- `dot_product_par` and `vector_component_sum_par` use OpenMP `reduction(+:sum)` to avoid race conditions on the shared accumulator

---

## Dependencies

- GCC with OpenMP (`-fopenmp`)
- Standard C library (`stdlib.h`, `stdio.h`, `math.h`, `time.h`)
- No external libraries
