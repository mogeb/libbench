//#include <libustperf.h>
#include <pthread.h>
#include <linux/perf_event.h>
#include <sys/mman.h>
#include <unistd.h>
#include "/home/mogeb/git/libbench/libustperf.h"

int do_work()
{
	int *a, i;

//	for (i = 0; i < 1000; i++) {
		a = malloc(32 * 1024);
//	}

	return 0;
}

///* perf_event_open syscall wrapper */
//static long
//sys_perf_event_open(struct perf_event_attr *hw_event,
//		    pid_t pid, int cpu, int group_fd, unsigned long flags)
//{
//    return syscall(__NR_perf_event_open, hw_event, pid, cpu, group_fd, flags);
//}

//#include <sys/syscall.h>
//static inline pid_t gettid()
//{
//    return syscall(SYS_gettid);
//}


//#include <sys/mman.h>
//struct perf_event_mmap_page *this_setup_perf(struct perf_event_attr *attr)
//{
//    void *perf_addr;
//    int fd, ret;
//    size_t pgsz;
//    pid_t tid = gettid();

//    pgsz = sysconf(_SC_PAGESIZE);

//    fd = sys_perf_event_open(attr, tid, -1, -1, 0);

//    if (fd < 0) {
//	printf("sys_perf_event_open fails\n");
//	return NULL;
//    }

//    perf_addr = mmap(NULL, pgsz, PROT_READ, MAP_SHARED, fd, 0);

//    if (perf_addr == MAP_FAILED)
//	return NULL;

//    ret = close(fd);
//    if (ret) {
//	perror("Error closing perf memory mapping FD");
//    }

//    return perf_addr;
//}

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

	for (i = 0; i < 100; i++) {
		int pos = cpu_perf[0].pos;
//		pmu1_start = mmap_read_self(perf_mmap1);
//		pmu2_start = mmap_read_self(perf_mmap2);
//		pmu3_start = mmap_read_self(perf_mmap3);
//		pmu4_start = mmap_read_self(perf_mmap4);

//		clock_gettime(CLOCK_MONOTONIC, &ts_start);
		LIB_PERF_ITER_START
		do_work();
		LIB_PERF_ITER_END
//		clock_gettime(CLOCK_MONOTONIC, &ts_end);
//		pmu4_end = mmap_read_self(perf_mmap4);
//		pmu1_end = mmap_read_self(perf_mmap1);
//		pmu2_end = mmap_read_self(perf_mmap2);
//		pmu3_end = mmap_read_self(perf_mmap3);

//		ts_diff = do_ts_diff(ts_start, ts_end);
//		cpu_perf[0].entries[pos].pmu1 = pmu1_end - pmu1_start;
//		cpu_perf[0].entries[pos].pmu2 = pmu2_end - pmu2_start;
//		cpu_perf[0].entries[pos].pmu3 = pmu3_end - pmu3_start;
//		cpu_perf[0].entries[pos].pmu4 = pmu4_end - pmu4_start;
//		cpu_perf[0].entries[pos].latency = ts_diff.tv_sec * 1000000000 +
//			ts_diff.tv_nsec;
//		printf("%llu\n", cpu_perf[0].entries[pos].latency);
//		cpu_perf[0].pos++;
//		cpu_perf[0].pos = cpu_perf[0].pos % PER_CPU_ALLOC;
	}

	LIB_PERF_END
}

//void this_perf_init(int nCpus)
//{
//    int i;

//    cpu_perf = malloc(nCpus * sizeof(struct measurement_cpu_perf));

//    for(i = 0; i < nCpus; i++) {
//	cpu_perf[i].entries = (struct measurement_entry*) malloc(PER_CPU_ALLOC *
//		    sizeof(struct measurement_entry));
//	cpu_perf[i].pos = 0;
//    }

//    attr1 = (struct perf_event_attr *) malloc(sizeof(struct perf_event_attr));
//    attr2 = (struct perf_event_attr *) malloc(sizeof(struct perf_event_attr));
//    attr3 = (struct perf_event_attr *) malloc(sizeof(struct perf_event_attr));
//    attr4 = (struct perf_event_attr *) malloc(sizeof(struct perf_event_attr));

//    /**
//      WARNING: LLC MISSES CRASHES!!!
//    **/
//    /*
//    attr2.size = sizeof(struct perf_event_attr);
//    attr2.pinned = 1;
//    attr2.disabled = 0;
//    attr2.type = PERF_TYPE_HW_CACHE;
//    attr2.config = PERF_COUNT_HW_CACHE_LL | \
//	       PERF_COUNT_HW_CACHE_OP_READ << 8 | \
//	       PERF_COUNT_HW_CACHE_RESULT_MISS << 16;
//    strncat(metric2, "LLC_misses", METRIC_LEN);
//    */
//    /**
//      WARNING: LLC MISSES CRASHES!!!
//    **/

//#if TRACK_PMU_MISSES
//    attr1->size = sizeof(struct perf_event_attr);
//    attr1->pinned = 1;
//    attr1->disabled = 0;
//    attr1->type = PERF_TYPE_HW_CACHE;
//    attr1->config = PERF_COUNT_HW_CACHE_L1D | \
//	       PERF_COUNT_HW_CACHE_OP_READ << 8 | \
//	       PERF_COUNT_HW_CACHE_RESULT_MISS << 16;
//    attr1->read_format = PERF_FORMAT_GROUP|PERF_FORMAT_ID;
//    strncat(metric1_str, "L1_misses", METRIC_LEN);

//    /* attr2 = cache misses */
//    attr2->size = sizeof(struct perf_event_attr);
//    attr2->pinned = 1;
//    attr2->disabled = 0;
//    attr2->type = PERF_TYPE_HARDWARE;
//    attr2->config = PERF_COUNT_HW_CACHE_MISSES;
//    attr2->read_format = PERF_FORMAT_GROUP|PERF_FORMAT_ID;
//    strncat(metric2_str, "Cache_misses", METRIC_LEN);

//    attr3->size = sizeof(struct perf_event_attr);
//    attr3->pinned = 1;
//    attr3->disabled = 0;
//    attr3->type = PERF_TYPE_HARDWARE;
//    attr3->config = PERF_COUNT_HW_CPU_CYCLES;
//    attr3->read_format = PERF_FORMAT_GROUP|PERF_FORMAT_ID;
//    strncat(metric3_str, "CPU_cycles", METRIC_LEN);

//    attr4->size = sizeof(struct perf_event_attr);
//    attr4->pinned = 1;
//    attr4->disabled = 0;
//    attr4->type = PERF_TYPE_HARDWARE;
//    attr4->config = PERF_COUNT_HW_INSTRUCTIONS;
//    attr4->read_format = PERF_FORMAT_GROUP|PERF_FORMAT_ID;
//    strncat(metric4_str, "Instructions", METRIC_LEN);

//#else // not TRACK_PMU_MISSES

//    /* attr4 = dTLB-load-misses */
//    attr1.size = sizeof(struct perf_event_attr);
//    attr1.pinned = 1;
//    attr1.disabled = 0;
//    attr1.type = PERF_TYPE_HW_CACHE;
//    attr1.config = PERF_COUNT_HW_CACHE_DTLB | \
//	       PERF_COUNT_HW_CACHE_OP_READ << 8 | \
//	       PERF_COUNT_HW_CACHE_RESULT_MISS << 16;
//    attr1.read_format = PERF_FORMAT_GROUP|PERF_FORMAT_ID;
//    strncat(metric1_str, "TLB_misses", METRIC_LEN);

//    attr2.size = sizeof(struct perf_event_attr);
//    attr2.pinned = 1;
//    attr2.disabled = 0;
//    attr2.type = PERF_TYPE_HARDWARE;
//    attr2.config = PERF_COUNT_HW_BUS_CYCLES;
//    attr2.read_format = PERF_FORMAT_GROUP|PERF_FORMAT_ID;
//    strncat(metric2_str, "Bus_cycles", METRIC_LEN);

//    attr3.size = sizeof(struct perf_event_attr);
//    attr3.pinned = 1;
//    attr3.disabled = 0;
//    attr3.type = PERF_TYPE_HARDWARE;
//    attr3.config = PERF_COUNT_HW_BRANCH_MISSES;
//    attr3.read_format = PERF_FORMAT_GROUP|PERF_FORMAT_ID;
//    strncat(metric3_str, "Branch_misses", METRIC_LEN);

//    attr4.size = sizeof(struct perf_event_attr);
//    attr4.pinned = 1;
//    attr4.disabled = 0;
//    attr4.type = PERF_TYPE_HARDWARE;
//    attr4.config = PERF_COUNT_HW_BRANCH_INSTRUCTIONS;
//    attr4.read_format = PERF_FORMAT_GROUP|PERF_FORMAT_ID;
//    strncat(metric4_str, "Branch_instructions", METRIC_LEN);

//#endif // TRACK_PMU_MISSES
//}

//void output_measurements(int nCpus)
//{
//    int i, cpu;
//    FILE *outfile;

//    outfile = fopen("/tmp/out.csv", "w+");
//    fprintf(outfile, "latency,%s,%s,%s,%s\n", metric1_str, metric2_str,
//	    metric3_str, metric4_str);
//    for(cpu = 0; cpu < nCpus; cpu++) {
//	for(i = 0; i < cpu_perf[cpu].pos; i++) {
//	    fprintf(outfile, "%lu,%lu,%lu,%lu,%lu\n",
//		    cpu_perf[cpu].entries[i].latency,
//		    cpu_perf[cpu].entries[i].pmu1,
//		    cpu_perf[cpu].entries[i].pmu2,
//		    cpu_perf[cpu].entries[i].pmu3,
//		    cpu_perf[cpu].entries[i].pmu4);
//	}
//    }
//    fclose(outfile);
//}

int main()
{
	int i;
	pthread_t *threads;
	threads = (pthread_t*) malloc(sizeof(pthread_t));

//	this_perf_init(1);
	perf_init(1);

	for(i = 0; i < 1; i++) {

	    pthread_create(&threads[i], NULL, do_measurement, NULL);
	}

	for(i = 0; i < 1; i++) {
	    pthread_join(threads[i], NULL);
	}

//	do_measurement();

	return 0;
}
