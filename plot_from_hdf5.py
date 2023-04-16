from keyword import kwlist
import numpy as np, matplotlib.pyplot as plt
import h5py as h5
from multiprocessing import Pool, Process
from typing import List

class Config:
    extent = 400
    plot_inactive = False
    max_snapshots = None
    filename = 'output/output.h5part'
    n_procs = 40

def created_images(time_steps: List[int], f: h5.File) -> None:
    xmax = .5 * Config.extent
    t = time_steps[0]
    group_name = f'Step#{t}'
    x = f[group_name]['X'][:]
    y = f[group_name]['Y'][:]
    status = f[group_name]['STATUS'][:]

    fig = plt.figure(figsize=(10,10))
    ax = plt.axes((0, 0, 1, 1))
    plt.axis('off')
    if Config.plot_inactive:
        plot_inactive, = plt.plot(x[status==0], y[status==0], '.', ms=3, alpha=.25, c=(.95,.95,.95))
    plot_active, = plt.plot(x[status==1], y[status==1], '.', ms=3, alpha=.25, c='C0')
    ax.set_aspect('equal')
    plt.xlim(-xmax,xmax)
    plt.ylim(-xmax,xmax)
    plt.savefig(f'images/{t:04d}.png')

    for t in time_steps[1:]:
        group_name = f'Step#{t}'
        print(group_name)
        x = f[group_name]['X'][:]
        y = f[group_name]['Y'][:]
        status = f[group_name]['STATUS'][:]
        if Config.plot_inactive:
            plot_inactive.set_xdata(x[status==0])
            plot_inactive.set_ydata(y[status==0])
        plot_active.set_xdata(x[status==1])
        plot_active.set_ydata(y[status==1])
        plt.draw()
        plt.savefig(f'images/{t:04d}.png')

if __name__ == '__main__':
    f = h5.File(Config.filename, 'r')
    groups = list(f.keys())
    time_steps = [int(group[5:]) for group in groups]
    time_steps = sorted(time_steps)
    
    if Config.max_snapshots:
        time_steps = time_steps[:Config.max_snapshots]

    if len(time_steps) <= Config.n_procs:
        step_lists = [time_steps]
        Config.n_procs = 1
    else:
        step_lists = []
        for i in range(Config.n_procs):
            step_lists.append(time_steps[i::Config.n_procs])

    processes = []
    for i in range(Config.n_procs):
        p = Process(target=created_images, args=(step_lists[i], f))
        p.start()
        processes.append(p)
    
    for p in processes:
        p.join()