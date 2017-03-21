#include <stdlib.h>
#include <pthread.h>
#include <linux/perf_event.h>
#include <sys/mman.h>
#include <unistd.h>
#include <libustperf.h>

int do_work()
{
	int *a;

	a = malloc(1 * 1024);

	return 0;
}

int *do_measurement()
{
	printf("do_measurement\n");
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
		int pos = cpu_perf[0].pos;
		LIB_PERF_ITER_START
		do_work();
		LIB_PERF_ITER_END
	}

	LIB_PERF_END
}

int main()
{
	int i;
	pthread_t *threads;

	threads = (pthread_t*) malloc(sizeof(pthread_t));
	perf_init(1);
	for(i = 0; i < 1; i++) {
	    pthread_create(&threads[i], NULL, do_measurement, NULL);
	}

	for(i = 0; i < 1; i++) {
	    pthread_join(threads[i], NULL);
	}

	return 0;
}
