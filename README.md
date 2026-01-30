# Dark Cloud Seed Simulator

This project contains a C++ program for simulating and analyzing seed values, outputting results to a CSV file.

## Files
- `dc_seed_sim.cpp`: Main source code for the simulation.

## Compilation

To compile the program, use g++ (or any C++ compiler):

```sh
g++ -O2 -std=c++17 dc_seed_sim.cpp -o dc_seed_sim
```

This will produce an executable named `dc_seed_sim` in the current directory.

## Usage

Run the program from the terminal with the following arguments:

```
./dc_seed_sim <seed> <iterations>
```

- `<seed>`: Initial seed value (e.g., `0x31D2DE92`)
- `<iterations>`: Number of iterations to simulate (e.g., `2000000`)

Example:

```
./dc_seed_sim 0x31D2DE92 2000000
```

The results will be saved to `seed_results.csv` in the same directory.

## Requirements
- C++ compiler (g++, clang++, etc.)
- macOS, Linux, or Windows

## Output
- The program generates a CSV file (`seed_results.csv`) containing the results of seed values after simulation.
