#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <strings.h>
#include <errno.h>
#include <selinux/selinux.h>

static void usage(const char *progname)
{
    fprintf(stderr, "usage:  %s [ Enforcing | Permissive | 0 | 1 ]\n",
            progname);
    exit(1);
}

int setenforce_main(int argc, char **argv)
{
    int rc = 0;
    if (argc != 2) {
        usage(argv[0]);
    }

    if (is_selinux_enabled() <= 0) {
        fprintf(stderr, "%s: SELinux is disabled\n", argv[0]);
        return 1;
    }
    if (strlen(argv[1]) == 1 && (argv[1][0] == '0' || argv[1][0] == '1')) {
        rc = security_setenforce(atoi(argv[1]));
    } else {
        if (strcasecmp(argv[1], "enforcing") == 0) {
            rc = security_setenforce(1);
        } else if (strcasecmp(argv[1], "permissive") == 0) {
            rc = security_setenforce(0);
        } else
            usage(argv[0]);
    }
    if (rc < 0) {
        fprintf(stderr, "%s:  Could not set enforcing status:  %s\n",
                argv[0], strerror(errno));
        return 2;
    }
    return 0;
}
