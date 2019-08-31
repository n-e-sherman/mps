import numpy as np
import matplotlib.pyplot as plt
import copy


def binomial(n, k):
    return 1 if k == 0 else (0 if n == 0 else binomial(n - 1, k) + binomial(n - 1, k - 1))


def euler(ys):
    res = copy.deepcopy(ys)
    for i in range(len(ys)):
        print(i)
        for x in range(len(ys[i])):
            temp = 0
            for j in range(i + 1):
                temp += binomial(i, j) * ys[i][x]
            res[i][x] = (0.5**(i + 1)) * temp


def nlc(Ls, xs, ys):
    weights = [ys[0]]  # [L][w]
    for i in range(1, len(ys)):
        if(i % 2 == 0):
            weights.append(ys[i])
            for j in range(i):
                for x in range(len(weights[i])):
                    weights[i][x] -= (i + 1 - j) * weights[j][x]
        else:
            weights.append([0 for x in ys[i]])

    res = copy.deepcopy(weights)
    for i in range(len(weights)):
        for j in range(i):
            for x in range(len(weights[i])):
                res[i][x] += weights[j][x]

    nres = np.swapaxes(np.array(res), 0, 1)
    print(nres.shape)
    # for y in nres:
    #     plt.plot(Ls, y)
    #     plt.show()
    # for y in res:
    # eres = euler(weights)
    N = 6
    for i in range(0, N, 2):
        plt.plot(xs[0], res[-i], label=str(i))
    # plt.plot(xs[-1], ys[-1], label='bare')
    plt.legend()

    # N = 4
    # for i in range(N):
    #     plt.plot(xs[0], res[-i])
    plt.show()
    # nweights = np.swapaxes(np.array(weights), 0, 1)
    # nxs = np.swapaxes(np.array(xs), 0, 1)
    # for x, y in zip(nxs, nweights):
    #     plt.scatter(x, y)
    #     plt.show()
