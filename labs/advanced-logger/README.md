Lab  - Advanced Logger
======================

Now it's time to add support for Linux system logging into your logger that you implemented on [logger](https://github.com/CodersSquad/ap-labs/tree/master/labs/logger).

A new `initLogger`function will be added in order to add support for choosing between `stdout` or `syslog` logging.

```
int initLogger(char *logType)
```

You will handle the following log types:
- `stdout` or empty string for `STDOUT` logging
- `syslog` for `SYSLOG` logging

You can use the **The Linux Programming Interface** book as a reference for your implementation. See *37th chapter on 5th section*.

General Instructions
--------------------
- Use the `testLogger.c` file for testing your `logger.c` library implementation.
- You can update the `Makefile` for your compilation and linking.
- Update `README.md` with the proper steps for building and running your code.
- Don't forget to handle errors properly.
- Coding best practices implementation will be also considered.

Test Suite
----------
Build and Test automation is already implemented with the following command. Below some general tips and comments.

- Make sure that your program passes all test cases without errors.
- Remember that this is being executed by a robot script.
- You cannot edit the `lab.mk` file.
- Failed compilation or segmentation faults means 0-graded.
- Failed tests without proper handling  will be properly discounted from total grade.

```
make test

// HOW TO EXECUTE CODE
---------------------------
- in the terminal type this commands

1. gcc logger.c testLogger.c -o exec.o

2. ./exec.o [# number of test]
```

How to submit your work
=======================
```
make submit
```
More details at: [Classify API](../../classify.md)
