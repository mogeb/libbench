#ifndef LIBUST_PERF_H
#define LIBUST_PERF_H

#include <asm/unistd.h>
#include <linux/perf_event.h>
#include <popt.h>
#include <sys/syscall.h>
#include <time.h>
#include <utils.h>

struct libustperf_args {
    int id;
    int loops;
};

struct measurement_entry {
    unsigned long pmu1;
    unsigned long pmu2;
    unsigned long pmu3;
    unsigned long pmu4;
    unsigned long latency;
};

struct measurement_cpu_perf {
    struct measurement_entry *entries;
    unsigned int pos;
};

struct perf_event_attr *attr1, *attr2, *attr3, *attr4;
struct measurement_cpu_perf *cpu_perf;

char metric1_str[METRIC_LEN];
char metric2_str[METRIC_LEN];
char metric3_str[METRIC_LEN];
char metric4_str[METRIC_LEN];

void output_measurements(int nCpus);
void perf_init(int nCpus);
int enable_misses_pmus();
int enable_branches_pmus();
struct perf_event_mmap_page *setup_perf(struct perf_event_attr *attr);

/* perf_event_open syscall wrapper */
static inline long
sys_perf_event_open(struct perf_event_attr *hw_event,
		    pid_t pid, int cpu, int group_fd, unsigned long flags)
{
    return syscall(__NR_perf_event_open, hw_event, pid, cpu, group_fd, flags);
}

static inline pid_t gettid()
{
    return syscall(SYS_gettid);
}

static unsigned long long rdpmc(unsigned int counter) {
  unsigned int low, high;

  __asm__ volatile("rdpmc" : "=a" (low), "=d" (high) : "c" (counter));

  return (unsigned long long)low | ((unsigned long long)high) <<32;
}

#define barrier() __asm__ volatile("" ::: "memory")

/* Ingo's code for using rdpmc */
static inline unsigned long long mmap_read_self(struct perf_event_mmap_page *addr) {

    struct perf_event_mmap_page *pc = addr;
    unsigned int seq,idx;

    unsigned long long count;

    do {
        seq = pc->lock;
        barrier();

        idx = pc->index;
        count = pc->offset;

        if (idx) {
            count += rdpmc(pc->index-1);
        }
        barrier();
    } while (pc->lock != seq);

    return count;
}

#define per_thread_init do \
	{ \
	perf_mmap1 = setup_perf(attr1); \
	if(!perf_mmap1) { \
	    printf("Couldn't allocate perf_mmap1\n"); \
	} \
	perf_mmap2 = setup_perf(attr2); \
	if(!perf_mmap2) { \
	    printf("Couldn't allocate perf_mmap2\n"); \
	} \
	perf_mmap3 = setup_perf(attr3); \
	if(!perf_mmap3) { \
	    printf("Couldn't allocate perf_mmap3\n"); \
	} \
	perf_mmap4 = setup_perf(attr4); \
	if(!perf_mmap4) { \
	    printf("Couldn't allocate perf_mmap4\n"); \
	} \
	} while (0);

#define LIB_PERF_ITER_START         do \
	{ \
	int pos = cpu_perf[0].pos; \
	pmu1_start = mmap_read_self(perf_mmap1); \
	pmu2_start = mmap_read_self(perf_mmap2); \
	pmu3_start = mmap_read_self(perf_mmap3); \
	pmu4_start = mmap_read_self(perf_mmap4); \
	clock_gettime(CLOCK_MONOTONIC, &ts_start); \
	} while (0);

#define LIB_PERF_ITER_END do \
	{ \
	int __pos = cpu_perf[0].pos; \
	clock_gettime(CLOCK_MONOTONIC, &ts_end); \
	pmu4_end = mmap_read_self(perf_mmap4); \
	pmu1_end = mmap_read_self(perf_mmap1); \
	pmu2_end = mmap_read_self(perf_mmap2); \
	pmu3_end = mmap_read_self(perf_mmap3); \
	ts_diff = do_ts_diff(ts_start, ts_end); \
	cpu_perf[0].entries[__pos].pmu1 = pmu1_end - pmu1_start; \
	cpu_perf[0].entries[__pos].pmu2 = pmu2_end - pmu2_start; \
	cpu_perf[0].entries[__pos].pmu3 = pmu3_end - pmu3_start; \
	cpu_perf[0].entries[__pos].pmu4 = pmu4_end - pmu4_start; \
	cpu_perf[0].entries[__pos].latency = ts_diff.tv_sec * 1000000000 + \
		ts_diff.tv_nsec; \
	cpu_perf[0].pos++; \
	cpu_perf[0].pos = cpu_perf[0].pos % PER_CPU_ALLOC; \
	} while (0);

#define LIB_PERF_END do \
	{ \
	output_measurements(1); \
	} while (0);

#endif // LIBUST_PERF_H
