import matplotlib.pyplot as plt
from matplotlib import cm
import numpy as np
import subprocess
import signal
import os


def kill():
    p = subprocess.Popen(['ps', '-A'], stdout=subprocess.PIPE)
    out, err = p.communicate()
    for line in out.splitlines():
        if 'Python' in str(line):
            myid = os.getpid()
            pid = int(line.split(None, 1)[0])
            if pid != myid:
                os.kill(pid, signal.SIGKILL)


kill()


def list_files(startpath):
    for root, dirs, files in os.walk(startpath):
        level = root.replace(startpath, '').count(os.sep)
        indent = ' ' * 4 * (level)
        print('{}{}/'.format(indent, os.path.basename(root)))
        subindent = ' ' * 4 * (level + 1)
        for f in files:
            print('{}{}'.format(subindent, f))


list_files('results/Heisenberg_chain_32')
E0 = -13.9973
folder = 'results/Heisenberg_chain_32/512/1.0/weights/'
omegasBare = []
omegas = []
weights = []
residuals = []
for i in range(80):
    f = open(folder + str(i))
    tempOB = []
    tempO = []
    tempW = []
    tempR = []
    for line in f:
        words = line.split(',')
        tempOB.append(float(words[0]))
        tempO.append(float(words[1]) + 13.9973)
        tempW.append(float(words[2]))
        tempR.append(float(words[3]))
    omegasBare.append(tempOB)
    omegas.append(tempO)
    weights.append(tempW)
    residuals.append(tempR)

iterations = [i for i in range(len(omegas))]
for i, os, ws, rs in zip(iterations, omegas, weights, residuals):
    xs = np.array([i for x in range(len(os))])
    ys = np.array(os)
    zs = np.array(ws)
    inds = np.where(zs > 1e-2)
    # inds = np.array([i for i in range(len(os))])
    plt.scatter(xs[inds], ys[inds], c=zs[inds], s=5, cmap=cm.YlOrRd)
    # plt.scatter(xs[inds], ys[inds], c=zs[inds], s=5, cmap=cm.cool)
    # for o, w, r in zip(os, ws, rs):
    #     plt.scatter(i, o, s=1)
plt.colorbar()
plt.show()
