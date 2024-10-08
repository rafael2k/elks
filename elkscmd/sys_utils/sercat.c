/*
 * sercat.c - serial cat (for testing)
 *
 *  sercat [-v] [serial device] [> file]
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>
#include <termios.h>
#include <signal.h>

#define DEFAULT_PORT "/dev/ttyS0"
#define BUF_SIZE 4096
#define CTRL_D  04

#define errmsg(str) write(STDERR_FILENO, str, sizeof(str) - 1)
#define errstr(str) write(STDERR_FILENO, str, strlen(str))

int verbose = 0;
int fd;
char readbuf[BUF_SIZE];
struct termios org, new;

void sig_handler(int sig)
{
    errmsg("Interrupt\n");
    tcsetattr(fd, TCSAFLUSH, &org);
    close(fd);
    exit(1);
}

void copyfile(int ifd, int ofd)
{
    int n;

    while ((n = read(ifd, readbuf, BUF_SIZE)) > 0) {
        if (n == 1 && readbuf[0] == CTRL_D)
            return;
        if (verbose) {
            char *p = itoa(n);
            errstr(p);
            errmsg("  bytes read\n");
        }
        write(ofd, readbuf, n);
    }
    if (n < 0) perror("read");
}

int main(int argc, char **argv)
{
    char *port = NULL;
    char *tty;

    if (argc > 1 && !strcmp(argv[1], "-v")) {
        verbose = 1;
        argc--;
        argv++;
    }
    if (argc > 1)
        port = argv[1];
    else {
        /* default to /dev/ttyS0 if not run from serial port and no argument */
        if (strncmp(ttyname(STDIN_FILENO), "/dev/ttyS", 9) != 0)
            port = "/dev/ttyS0";
    }
    if (port) {
        if ((fd = open(port, O_RDONLY|O_EXCL)) < 0) {
            perror(port);
            return 1;
        }
    } else fd = STDIN_FILENO;
    tty = ttyname(fd);
    errmsg("Reading from ");
    errstr(tty);
    errmsg("\n");

    if (tcgetattr(fd, &org) >= 0) {
        new = org;
        new.c_lflag &= ~(ICANON | ISIG | ECHO | ECHOE | ECHONL);
        new.c_iflag &= ~(ICRNL);
        new.c_cflag |= CS8 | CREAD;
        new.c_cc[VMIN] = 255;           /* min bytes to read if VTIME = 0*/
        new.c_cc[VTIME] = 1;            /* intercharacter timeout if VMIN > 0, timeout if VMIN = 0*/
        tcsetattr(fd, TCSAFLUSH, &new);
    } else errmsg("Can't set termios\n");
    signal(SIGINT, sig_handler);

    copyfile(fd, STDOUT_FILENO);

    tcsetattr(fd, TCSAFLUSH, &org);
    close(fd);
    return 0;
}
