# Resolution-week_6-Coding

## What the project is about

This project is a small terminal-based graph plotter written in C. It evaluates
functions from user-provided coefficients and renders the result as an
ASCII graph in the terminal.

Supported functions:

- Linear functions: `y = bx + c` (set `a` to `0`)
- Quadratic functions: `y = ax^2 + bx + c`
- Sine functions: `y = a sin(bx) + c`
- Cosine functions: `y = a cos(bx) + c`
- Tangent functions: `y = a tan(bx) + c`

While a graph is displayed, you can pan horizontally and change the zoom
level:

- `a` / `d`: pan left / right
- `+` / `-`: zoom in / out
- `q`: return to the menu

## How to run it

### Requirements

You need a C compiler. GCC is recommended. On Windows, install MinGW-w64 or
another GCC distribution and make sure `gcc` is available in your terminal.

### Build

From the repository root, compile the program with:

```text
gcc src\plotter.c -o plotter.exe -lm
```

On macOS or Linux, use the equivalent path separator:

```text
gcc src/plotter.c -o plotter -lm
```

The `-lm` option links the standard math library used by the trigonometric
functions.

### Start the program

On Windows:

```text
.\plotter.exe
```

On macOS or Linux:

```text
./plotter
```

Choose `Enter Function` from the menu, select a function type, and enter the
values for `a`, `b`, and `c` when prompted. The repository also contains a
pre-built Windows executable at `plotter.exe`, which can be launched directly
if you do not need to rebuild the program.
