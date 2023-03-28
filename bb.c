#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int nums = 0;
int numi = 0;
int numo = 0;

struct i *is = NULL;
struct o *os = NULL;

struct i {
	char	s;
	char	r;
};

struct o {
	char	w;
	char	m;
	char	t;
#define MOVE(_m) ((m == 0) ? 'L' : ((_m == 1) ? 'N' : ((_m == 2) ? 'R' : '?')))
};

struct f {
	/* The machine function is a listing of the (w,m,t) actions
	 * assigned to the (s,r) situations, in the given order.
	 * These are the indexes of the assigned actions. */
	unsigned *o;
};


void
usage()
{
	fprintf(stderr, "usage: bb [-s steps] num\n");
}

void
pri(struct i* i)
{
	if (i)
		printf("%c%c", i->s, i->r);
}

void
pro(struct o* o)
{
	if (o)
		printf("%c%c%c", o->w, o->m, o->t);
}

void
prf(struct f* tm)
{
	int i;
	for (i = 0; i < numi; i++) {
		/*printf("%d: %d\n", i, tm->o[i]);*/
		pri(&is[i]);
		if (tm->o[i] < numo) {
			pro(&os[tm->o[i]]);
		} else {
			printf("%cNZ", is[i].r);
		}
		putchar('\n');
	}
}

/* Iterate to the next possible Turing machine,
 * shifting to the next listing of actions. */
int
next(struct f *tm)
{
	int j, i = numi - 1;
	while (i >= 0) {
		if (tm->o[i] < numo) {
			tm->o[i]++;
			/*printf("%d: bump to %d\n", i, tm->o[i]);*/
			return 1;
		}
		if (i == 0)
			return 0;
		for (j = i--; j < numi; j++)
			tm->o[j] = 0;
	}
	return 0;
}

int
main(int argc, char** argv)
{
	int c;
	int s, r;
	int w, m, t;
	struct f *tm;

	while ((c = getopt(argc, argv, "s:")) != -1) switch (c) {
		case 's':
			break;
		default:
			usage();
			return 1;
	}
	argc -= optind;
	argv += optind;

	if (argc != 1) {
		usage();
		return 1;
	}

	nums = strtonum(argv[0], 1, 26, NULL);
	/* (6n+1)^(2n) machines with n states */
	numi = nums * 2;
	numo = nums * 6;

	is = calloc(numi, sizeof(struct i));
	os = calloc(numo, sizeof(struct o));

	for (s = 0; s < nums; s++) {
		for (r = 0; r < 2; r++) {
			is[2*s + r].s = 'A' + s;
			is[2*s + r].r = '0' + r;
			/*
			printf("input %d: ", 2*s + r);
			pri(&is[2*s + r]);
			putchar('\n');
			*/
		}
	}

	for (w = 0; w < 2; w++) {
		for (m = 0; m < 3; m++) {
			for (t = 0; t < nums; t++) {
				os[3*w + m + 6*t].w = '0' + w;
				os[3*w + m + 6*t].m = MOVE(m);
				os[3*w + m + 6*t].t = 'A' + t;
				/*
				printf("action %d: ", 3*w + m + 6*t);
				pro(&os[3*w + m + 6*t]);
				putchar('\n');
				*/
			}
		}
	}

	tm = calloc(1, sizeof(struct f));
	tm->o = calloc(numi, sizeof(unsigned));

	do {
		/* XXX Save it into a file and run it */
		printf("machine:\n");
		prf(tm);
	} while (next(tm));

	return 0;
}
