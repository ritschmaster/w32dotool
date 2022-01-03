#ifndef W32DOTOOL_GETMOUSELOCATION_H
#define W32DOTOOL_GETMOUSELOCATION_H

#include "cmd.h"

typedef struct wdt_getmouselocation_s wdt_getmouselocation_t;

struct wdt_getmouselocation_s
{
    wdt_cmd_t cmd;

    int (* super_wdt_getmouselocation_free)(wdt_cmd_t *cmd);
    int (* wdt_getmouselocation_is_shell)(wdt_getmouselocation_t *getmouselocation);

    int shell;
};

extern wdt_getmouselocation_t *
wdt_getmouselocation_new(int argc, char *argv[]);

extern int
wdt_getmouselocation_is_shell(wdt_getmouselocation_t *getmouselocation);

#endif // W32DOTOOLGETMOUSELOCATION_H
