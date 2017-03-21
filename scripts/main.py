import sys
import numpy as np
import matplotlib.pylab as pylab
import matplotlib
from matplotlib.font_manager import FontProperties
from collections import defaultdict
from pylab import *

allocators_colors = {'malloc': 'r', 'tcmalloc': 'b'}


def do_metrics_scatter_plot():
    allocators = ['malloc', 'tcmalloc']
    values = defaultdict(list)

    for allocator in allocators:
        fname = allocator + '.csv'
        with open(fname, 'r') as f:
            legend = f.readline()
        legend = legend.split(',')
        values[allocator] = np.genfromtxt(fname, delimiter=',', skip_header=1,
            names=legend, dtype=None)
    fontP = FontProperties()
    fontP.set_size('small')

    i = 0
    for metric in legend:
        plt.subplot(2, 2, i)
        for allocator in allocators:
            if metric == 'latency':
                continue
            metric = metric.strip()
            plt.scatter(values[allocator][metric], values[allocator]['latency'],
                color=allocators_colors[allocator], alpha=0.3, label=allocator)
        plt.title('Latency according to ' + metric)
        plt.xlabel(metric)
        plt.ylabel('Latency in ns')
        i += 1

    plt.legend(loc=4, borderaxespad=0., fontsize='small')
    plt.show()


def do_cpi_scatter_plot():
    allocators = ['malloc', 'tcmalloc']
    values = defaultdict(list)
    cpi = defaultdict(list)
    ipc = defaultdict(list)

    for allocator in allocators:
        fname = allocator + '.csv'
        with open(fname, 'r') as f:
            legend = f.readline()
        values[allocator] = np.genfromtxt(fname, delimiter=',', skip_header=1,
            names=legend, dtype=int)

    for allocator in allocators:
        for i in range(0, len(values[allocator])):
            ipc[allocator].append(values[allocator]['Instructions']
                                  [i] / values[allocator]['CPU_cycles'][i])
            cpi[allocator].append(values[allocator]['CPU_cycles']
                                  [i] / values[allocator]['Instructions'][i])

    fontP = FontProperties()
    fontP.set_size('small')

    plt.subplot(2, 1, 0)
    plt.title('Latency according to CPI')
    plt.xlabel('CPI')
    plt.ylabel('Latency in ns')
    plt.legend(prop=fontP)
    for allocator in allocators:
        plt.scatter(cpi[allocator], values[allocator]['latency'],
                    color=allocators_colors[allocator], alpha=0.3, label=allocator)
    plt.legend(prop=fontP)

    plt.subplot(2, 1, 1)
    plt.title('Latency according to IPC')
    plt.xlabel('IPC')
    plt.ylabel('Latency in ns')
    plt.legend(prop=fontP)
    for allocator in allocators:
        plt.scatter(ipc[allocator], values[allocator]['latency'],
                    color=allocators_colors[allocator], alpha=0.3, label=allocator)

    plt.legend(prop=fontP)
    plt.show()


def main(args):
    do_metrics_scatter_plot()
    # do_cpi_scatter_plot()


if __name__ == '__main__':
    main(sys.argv)
