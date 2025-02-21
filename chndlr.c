/* See LICENSE file for copyright and license details. */
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <regex.h>
#include <unistd.h>

typedef struct {
	const char *regex;
	const char **action;
} Pair;

#include "config.h"

int i;
char cmd[BUFSIZ], *cmdv[BUFSIZ/16];
regmatch_t match[9];

int
reexec(char *uri, const char **args) {
	const char *arg;
	char *p = cmd;
	int len;

	while (*args) {
		arg = *args;
		cmdv[args - pairs[i].action] = p;

		while (*arg) {
			if (*arg == '%') {
				unsigned char nc = *(arg + 1);
				/* check if N in %N is between 0 and 9 */
				int group = nc - '0';
				if (group <= 9) {
					len = match[group].rm_eo - match[group].rm_so;
					snprintf(p, len + 1, "%.*s", len, uri + match[group].rm_so);
				} else /* if (nc == 's') */ {
					len = sprintf(p, "%s", uri);
				}
				arg += 2;
				p += len;
			} else {
				*p++ = *arg++;
			}
		}

		*p++ = '\0';
		args++;
	}

	return execvp(*cmdv, cmdv);
}


int
main(int argc, char *argv[]){
	regex_t regex;

	/* we only take one argument */
	if (argc != 2)
		return EXIT_FAILURE;

	/* check regex and launch action if it matches argv[1] */
	for (i=0; i < sizeof(pairs)/sizeof(*pairs); ++i) {
		if (regcomp(&regex, pairs[i].regex, REG_EXTENDED))
			fprintf(stderr, "invalid regex: %s\n", pairs[i].regex);

		if (regexec(&regex, argv[1], 9, match, 0) == 0) {
			regfree(&regex);
			return reexec(argv[1], pairs[i].action);
		}
	}

	regfree(&regex);

	/* alternatively, fall back to chndlr_fallback_cmd */
	sprintf(cmd, "%s%s", chndlr_fallback_cmd, argv[1]);
	system(cmd);
	return EXIT_SUCCESS;
}
