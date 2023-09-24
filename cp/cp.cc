#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <assert.h>
#include "cp.h"

static ssize_t 
my_write(int fd, const char *buffer, ssize_t buf_size)
{
        ssize_t nwrote = 0;
        ssize_t n = 0;

        while (nwrote < buf_size) {
                n = write(fd, buffer, buf_size - nwrote);
                if (n <= 0) {
                        return n;
                }

                nwrote += n;
        }

        return nwrote;
}

static int 
file_translator(int fd_in, int fd_out) 
{
        char buffer[BUF_SIZE];
        ssize_t nread = 0;

        while(true) {
                nread = read(fd_in, buffer, BUF_SIZE);
                if (nread <= 0)
                        break;

                ssize_t nwrote = my_write(fd_out, buffer, nread);
                if (nwrote < 0) {
                        perror("my_cat: could not write");
                        return -1;
                }
        }

        if (nread < 0) {
                perror("my_cat: could not read");
                return -1;
        }

        return 0;
}

static void 
option_switch(int opt)
{
        switch (opt) {
                case 'f':
                case 'i':
                case 'v':
                case 'p':
                case 'h':
                default: 
                        assert(0 && "The option in not implemented yet.");
        }
}

int 
parse(int argc, char *argv[])
{
        while(true) {
                int option_index = 0;
                int c = getopt_long(argc, argv, "fivph", 
                                    long_options, &option_index);
                if (c == -1)
                        break;

                option_switch(c);
        }

        return 0;
}


int
cp(const char *src, const char *dest)
{
        int fd_in = open(src, O_RDONLY);
        int fd_out = open(dest, O_RDWR | O_CREAT);  

        file_translator(fd_in, fd_out);

        return 0;
}

