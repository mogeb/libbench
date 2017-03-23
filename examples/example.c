#define _GNU_SOURCE
#include <stdlib.h>
#include <pthread.h>
#include <linux/perf_event.h>
#include <sys/mman.h>
#include <sched.h>
#include <unistd.h>
#include <libustperf.h>

struct popt_args
{
    int num_threads;
};

struct popt_args popt_args;

struct poptOption options[] = {
    {
        "threads", 't',
        POPT_ARG_INT | POPT_ARGFLAG_OPTIONAL,
        &popt_args.num_threads, 0, "num_threads"
    },
    POPT_AUTOHELP
};

static void parse_args(int argc, char **argv, poptContext *pc)
{
    int val;

    *pc = poptGetContext(NULL, argc, (const char **)argv, options, 0);

    if (argc < 2) {
        poptPrintUsage(*pc, stderr, 0);
        return;
    }

    while ((val = poptGetNextOpt(*pc)) >= 0) {
        printf("poptGetNextOpt returned val %d\n", val);
    }
}

int do_work()
{
	int *a;

	a = malloc(1 * 1024);

	return 0;
}

void *do_measurement(void *arg)
{
	printf("do_measurement %d\n", sched_getcpu());
	int i;
	unsigned long pmu1_start, pmu1_end;
	unsigned long pmu2_start, pmu2_end;
	unsigned long pmu3_start, pmu3_end;
	unsigned long pmu4_start, pmu4_end;
	struct timespec ts_start, ts_end, ts_diff;
	struct perf_event_mmap_page *perf_mmap1, *perf_mmap2, *perf_mmap3,
		*perf_mmap4;

	perf_mmap1 = setup_perf(attr1);
	if(!perf_mmap1) {
	    printf("Couldn't allocate perf_mmap1\n");
	}
	perf_mmap2 = setup_perf(attr2);
	if(!perf_mmap2) {
	    printf("Couldn't allocate perf_mmap2\n");
	}
	perf_mmap3 = setup_perf(attr3);
	if(!perf_mmap3) {
	    printf("Couldn't allocate perf_mmap3\n");
	}
	perf_mmap4 = setup_perf(attr4);
	if(!perf_mmap4) {
	    printf("Couldn't allocate perf_mmap4\n");
	}

	for (i = 0; i < 99999; i++) {
		LIB_PERF_ITER_START
		do_work();
		LIB_PERF_ITER_END
	}

	LIB_PERF_END
}

int main(int argc, char **argv)
{
	int i, num_cpus;
	pthread_t *threads;
	poptContext pc;

	popt_args.num_threads = 1;

	num_cpus = sysconf(_SC_NPROCESSORS_ONLN);
	threads = (pthread_t *)malloc(sizeof(pthread_t));
	parse_args(argc, argv, &pc);
	perf_init(num_cpus);
	enable_misses_pmus();

	for (i = 0; i < popt_args.num_threads; i++) {
        /* Set CPU affinity for each thread*/
	    cpu_set_t cpu_set;
	    pthread_attr_t attr;
	    CPU_ZERO(&cpu_set);
	    CPU_SET(i % num_cpus, &cpu_set);
	    pthread_attr_init(&attr);
	    pthread_attr_setaffinity_np(&attr, sizeof(cpu_set), &cpu_set);
        /* Create threads */
	    pthread_create(&threads[i], NULL, do_measurement, NULL);
	}

	for (i = 0; i < 1; i++) {
	    pthread_join(threads[i], NULL);
	}

	output_measurements(num_cpus);

	return 0;
}
