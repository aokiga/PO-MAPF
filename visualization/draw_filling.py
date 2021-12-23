import matplotlib.pyplot as plt
from matplotlib import colors as mcolors
import random

colors = dict(mcolors.BASE_COLORS, **mcolors.CSS4_COLORS)

# Sort colors by hue, saturation, value and name.
by_hsv = sorted((tuple(mcolors.rgb_to_hsv(mcolors.to_rgba(color)[:3])), name)
                for name, color in colors.items())
names = [name for hsv, name in by_hsv if name not in {'black', 'k', 'w', 'white', 'crimson', 'royalblue', 'limegreen', 'yellow', 'orange'}]

random.shuffle(names)
names = ['white', 'black', 'crimson', 'royalblue', 'limegreen', 'yellow', 'orange', *names, 'red']


def fill_background(i, j, ax, cell, n):
    ax.fill([i, i, i + 1, i + 1, i], [j, j + 1, j + 1, j, j], color=names[0])
    if cell.background:
        ax.fill([i, i, i + 1, i + 1, i], [j, j + 1, j + 1, j, j], color=names[cell.background])


def fill_path(i, j, ax, cell, n):
    if cell.path is not None:
        for pos, C in enumerate(cell.path):
            if not pos:
                continue
            pj, pi = cell.path[pos - 1]
            pj = n - pj - 1
            cj, ci = C
            cj = n - cj - 1
            line = plt.Line2D([pi + 0.5, ci + 0.5], [pj + 0.5, cj + 0.5], color=names[cell.robot_color], zorder=5.0)
            ax.add_line(line)


def fill_robots(i, j, ax, cell, n):
    if cell.robot_color:
        circle = plt.Circle(
            [i + 0.5, j + 0.5],
            0.25,
            facecolor=names[cell.robot_color],
            linewidth=1,
            edgecolor='black',
            zorder=10.0
        )
        ax.add_patch(circle)


def fill(filling, n, ax, f):
    for i, row in enumerate(filling):
        i = n - i - 1
        for j, cell in enumerate(row):
            f(j, i, ax, cell, n)


def draw_filling(filling, horizonal_scale=0.75, vertical_scale=0.75):
    if filling is not None:
        n = len(filling)
        m = len(filling[0])
        fig = plt.figure(figsize=(min(m, 10), min(n, 10)))#m * vertical_scale, n * horizonal_scale))

        ax = fig.add_axes([0, 0, 1, 1])
        ax.get_xaxis().set_visible(False)
        ax.get_yaxis().set_visible(False)

        for name, spine in ax.spines.items():
            spine.set_visible(False)
            spine.set_visible(False)

        fill(filling, n, ax, fill_background)
        fill(filling, n, ax, fill_path)
        fill(filling, n, ax, fill_robots)

        for i in range(n + 1):
            ax.plot([0, m], [i, i], color='black')
        for i in range(m + 1):
            ax.plot([i, i], [0, n], color='black')
        plt.close(fig)
        return fig
    else:
        return None