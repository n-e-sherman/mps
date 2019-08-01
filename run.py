import os


def write(inputs, fname, table=""):
    f = open(fname, 'w+')
    for k, v in inputs.items():
        f.write(str(k) + " = " + str(v) + '\n')
    f.write(table)


# inputs = {
#     "wi": 1.0,
#     "wf": 3.0,
#     "n": 101
# }

inputs = {
    "dmrg": "no",
    "calcT": "no",
    "calcCor": "no",
    "model": "Heisenberg",
    "lattice": "chain",
    "N": "64",
    "nsweeps": "5",
    "nLanczos": "40",
    "wi": "0",
    "wf": "3.0",
    "nw": "101",
    "etas": "0.1,0.2",
    "qfactor": "1.0",
    "maxDim": "200",
    "quiet": "no",
    "reortho": "yes"
}
inputs["cwd"] = os.getcwd() + "/"
table = '''
sweeps
    {
    maxdim  mindim  cutoff  niter  noise
    40      20      1E-6    4      1E-7
    80      20      1E-8    3      1E-8
    100     10      1E-10   2      1E-10
    200     10      1E-12   2      0
    200     10      1E-12   2      0
    }
'''

for k, v in inputs.items():
    print(k, v)

write(inputs, 'trash', table)
