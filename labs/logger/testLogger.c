int infof(const char *format, ...);
int warnf(const char *format, ...);
int errorf(const char *format, ...);
int panicf(const char *format, ...);

int main() {

    infof("this is the information of something %d\n", 4);

    errorf("this is the error of something %d\n", 23);

    panicf("esto es panic mode %d\n", 25);

    warnf("this is a warning %d\n", 5);

    return 0;
}
