import sys
import numpy as np
import matplotlib.pylab as pylab
import matplotlib
from pylab import *
from matplotlib.font_manager import FontProperties
from collections import defaultdict
from pylab import *

allocators_colors = { 'malloc': 'r', 'tcmalloc': 'b'}

def main(args):
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


if __name__ == '__main__':
    main(sys.argv)
