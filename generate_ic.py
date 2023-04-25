import numpy as np
from numpy import double
import sys
import argparse

def concetric_circles(r_max: double) -> np.ndarray:
    x, y = np.array([0]), np.array([0])
    r_max_int = int(r_max)
    for r in range(1, r_max_int):
        n = int(2*np.pi*r)
        i = np.arange(n)
        x_ = r*np.cos(2*np.pi*i/n)
        y_ = r*np.sin(2*np.pi*i/n)
        x = np.append(x, x_)
        y = np.append(y, y_)
    r = np.sqrt(x**2 + y**2)
    r[r==0] = 1
    vx = .125*y/r * (0.95 + np.random.rand(len(x))*.10)
    vy = .250*x/r * (0.95 + np.random.rand(len(x))*.10)
    result = np.array([x, y, vx, vy])
    return result

def random_uniform_distribution(count: int, extent: double, velocity_dispersion: double):
    x  = (np.random.rand(count) - .5) * extent
    y  = (np.random.rand(count) - .5) * extent
    vx = np.random.randn(count) * velocity_dispersion
    vy = np.random.randn(count) * velocity_dispersion
    result = np.array([x, y, vx, vy])
    return result

def uniform_grid(extent: double, density: double = 1):
    x_ = np.linspace(-extent//2+.5, extent//2-.5, int(extent * np.sqrt(density)))
    y_ = x_
    x, y = np.meshgrid(x_, y_)
    x = x.flatten()
    count = len(x)
    y = y.flatten()
    vx = np.zeros(count)
    vy = np.zeros(count)
    result = np.array([x, y, vx, vy])
    return result

def random_normal_distribution(count: int, spatial_dispersion: double, velocity_dispersion: double):
    x  = np.random.randn(count) * spatial_dispersion
    y  = np.random.randn(count) * spatial_dispersion
    vx = np.random.randn(count) * velocity_dispersion
    vy = np.random.randn(count) * velocity_dispersion
    result = np.array([x, y, vx, vy])
    return result

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('--seed', type=int)
    parser.add_argument('--output', type=str, default='initial_conditions.dat')
    parser.add_argument('--density', type=float, default=1)
    parser.add_argument('--sigma', type=float, default=0)
    args = parser.parse_args()
    seed = args.seed if not args.seed is None else 0

    np.random.seed(seed)

    x, y, vx, vy =random_uniform_distribution(int(args.density * 400**2), extent = 400, velocity_dispersion = args.sigma)

    count = len(x)
    #status = np.random.randint(0, 2, count)
    status = np.zeros(count, dtype=int)
    x      = np.append(x, np.linspace(-49.5, 49.5, 100))
    y      = np.append(y, np.zeros(100))
    vx     = np.append(vx, np.zeros(100))
    vy     = np.append(vy, np.zeros(100))
    status = np.append(status, np.ones(100, dtype=int))
    count = len(x)

    # x, y, vx, vy = concetric_circles(r_max=200)
    # count = len(x)
    # status = np.zeros(count, dtype=int)
    # y_min = abs(y).min()
    # status[(-100 < x) & (x < 100) & (y == y_min)] = 1

    with open(args.output, 'w') as f:
        for i in range(count):
            f.write(f'{x[i]:13.6e}{y[i]:14.6e}{vx[i]:14.6e}{vy[i]:14.6e}  {status[i]}')
            if i < len(x) - 1: f.write('\n')