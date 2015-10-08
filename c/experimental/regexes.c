#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <regex.h>

// Example:
// $ a.out 'a.*b(cd).*(f).[hz](i)' 'abcdefghij'
// rc=0
// pmatch[0].rm_so=   0 pmatch[0].rm_eo=   9
// pmatch[1].rm_so=   2 pmatch[1].rm_eo=   4
// pmatch[2].rm_so=   5 pmatch[2].rm_eo=   6
// pmatch[3].rm_so=   8 pmatch[3].rm_eo=   9
// pmatch[4].rm_so=  -1 pmatch[4].rm_eo=  -1
// pmatch[5].rm_so=  -1 pmatch[5].rm_eo=  -1
// pmatch[6].rm_so=  -1 pmatch[6].rm_eo=  -1
// pmatch[7].rm_so=  -1 pmatch[7].rm_eo=  -1
// pmatch[8].rm_so=  -1 pmatch[8].rm_eo=  -1
// pmatch[9].rm_so=  -1 pmatch[9].rm_eo=  -1

static void usage(char* argv0, FILE* o, int exit_code) {
	fprintf(o, "Usage: %s {regex} {string}\n", argv0);
	exit(exit_code);
}

int main(int argc, char** argv) {
	if (argc >= 2 && (!strcmp(argv[1], "-h") || !strcmp(argv[1], "--help"))) {
		usage(argv[0], stdout, 0);
	}
	if (argc != 3) {
		usage(argv[0], stderr, 1);
	}
	regex_t reg;

	char* sregex = argv[1];
	char* sstr   = argv[2];
	int cflags = REG_EXTENDED;
	const size_t nmatch = 10;
	regmatch_t pmatch[nmatch];
	int eflags = 0;
	int rc;

	rc = regcomp(&reg, sregex, cflags);
	if (rc != 0) {
		size_t nbytes = regerror(rc, &reg, NULL, 0);
		char* errbuf = malloc(nbytes);
		(void)regerror(rc, &reg, errbuf, nbytes);
		printf("regcomp failure: %s\n", errbuf);
		exit(1);
	}

	rc = regexec(&reg, sstr, nmatch, pmatch, eflags);
	printf("rc=%d\n", rc);
	if (rc == 0) {
		for (int i = 0; i < nmatch; i++) {
			printf("pmatch[%i].rm_so=%4lld pmatch[%d].rm_eo=%4lld\n",
				i, (long long)pmatch[i].rm_so,
				i, (long long)pmatch[i].rm_eo);
		}
	} else if (rc == REG_NOMATCH) {
		printf("no match\n");
	} else {
		size_t nbytes = regerror(rc, &reg, NULL, 0);
		char* errbuf = malloc(nbytes);
		(void)regerror(rc, &reg, errbuf, nbytes);
		printf("regexec failure: %s\n", errbuf);
		exit(1);
	}

	regfree(&reg);

	return 0;
}

