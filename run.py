import os


def writeInput(inputs, fname, table=""):
    f = open(fname, 'w+')
    for k, v in inputs.items():
        f.write(str(k) + " = " + str(v) + '\n')
    f.write(table)


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
    "wf": "4.0",
    "nw": "201",
    "etas": "0.1,0.2",
    "qfactor": "1.0",
    "maxDim": "500",
    "quiet": "no",
    "reortho": "yes"
}
cwd = os.getcwd()
inputs["cwd"] = cwd + "/"
table = '''
sweeps
    {
    maxdim  mindim  cutoff  niter  noise
    80      20      1E-6    4      1E-7
    100      20      1E-8    3      1E-8
    150     10      1E-10   2      1E-10
    250     10      1E-12   2      0
    400     10      1E-12   2      0
    }
'''

W = 4.0  # roughly width of spectrum
Ns = ['20', '40', '60', '80', '90', '100']
nLs = ['40', '60', '80', '100', '100', '100']
cores = ['4', '4', '4', '8', '8', '8']
if not os.path.exists(cwd + '/inputs/'):
    os.mkdir('inputs/')
for N, nL in zip(Ns, nLs):
    label = N + nL
    inputs['N'] = N
    inputs['nLanczos'] = nL
    inputs['etas'] = '0.1' + ',' + str(W / float(N))
    writeInput(inputs, cwd + '/inputs/' + label, table)

    # write and run sbatch script here.
