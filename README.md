# CS744 DECS Shell Assignment

A simple Unix shell implemented in C for the **CS744 – DECS** assignment.

### Features

* Executes commands using `fork()` and `execvp()`
* Supports `cd` and `exit`
* Supports background processes using `&`
* Tracks background child processes using a linked list
* Handles `SIGINT` (`Ctrl+C`)
* Reaps completed background processes

### Compile & Run

```bash
gcc shell.c -o shell
./shell
```

### Example

```bash
$ pwd
$ sleep 5 &
$ ls
$ cd ..
$ exit
```
