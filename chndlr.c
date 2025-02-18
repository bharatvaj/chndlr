/* See LICENSE file for copyright and license details. */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <regex.h>

typedef struct {
	const char *regex;
	const char *action;
} Pair;

#include "config.h"

int
main(int argc, char *argv[]){
	int g, h, i;
	char cmd[BUFSIZ], sharg[BUFSIZ];
	memset(cmd, '\0', BUFSIZ);
	regex_t regex;
	regmatch_t match[9];

	/* we only take one argument */
	if (argc != 2)
		return EXIT_FAILURE;

	/* make the argument shell-ready
	 *   1) start with '
	 *   2) escape ' to '\''
	 *   3) close with '\0
	 */
	sharg[0] = '\'';
	for (g=0, h=1; argv[1][g] && h < BUFSIZ-1-3-2; ++g, ++h) {
		sharg[h] = argv[1][g];
		if (argv[1][g] == '\'') {
			sharg[++h] = '\\';
			sharg[++h] = '\'';
			sharg[++h] = '\'';
		}
	}
	sharg[h] = '\'';
	sharg[++h] = 0;

	int len;
	/* check regex and launch action if it matches argv[1] */
	for (i=0; i < sizeof(pairs)/sizeof(*pairs); ++i) {
		if (regcomp(&regex, pairs[i].regex, REG_EXTENDED))
			fprintf(stderr, "invalid regex: %s\n", pairs[i].regex);

		if (regexec(&regex, argv[1], 9, match, 0) == 0) {
			const char *action = pairs[i].action;
			char *p = cmd;

			while (*action) {
				/* check if N in %N is between 0 and 9 */
				if (*action == '%'){
					unsigned char nc = *(action + 1);
					if (nc - '0' <= 9) {
						int group = nc - '0';
						len = match[group].rm_eo - match[group].rm_so;
						snprintf(p, len + 1, "%.*s", len, argv[1] + match[group].rm_so);
					} else /* if (nc == 's') */ {
						len = strlen(argv[1]);
						snprintf(p, len + 1, "%.*s", len, argv[1]);
					}
					action += 2;
					p += len;
				} else {
					*p++ = *action++;
				}
			}
			*p = '\0';

			printf("Executing: %s\n", cmd);
			system(cmd);
			return EXIT_SUCCESS;
		}
		/*
		else {
			printf("Regex no match %s\n", pairs[i].regex);
		}
		*/
		regfree(&regex);
	}

	/* alternatively, fall back to SOAP_BROWSER */
	snprintf(cmd, sizeof cmd, CHNDLR_FALLBACK_CMD " %s", sharg);
	system(cmd);
	return EXIT_SUCCESS;
}
