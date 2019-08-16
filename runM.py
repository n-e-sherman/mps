import os


def writeInput(inputs, fname, table=""):
    f = open(fname, 'w+')
    f.write('input{\n')
    for k, v in inputs.items():
        f.write('\t' + str(k) + " = " + str(v) + '\n')
    f.write(table)
    f.write('\n}')


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
    "maxDim": "512",
    "quiet": "no",
    "reortho": "yes",
    "quantity": "weights"
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
    150     10      1E-12   2      0
    200     10      1E-12   2      0
    }
'''

baseSlurm = '''#!/bin/bash -l
#SBATCH --qos=regular
#SBATCH --nodes=6
#SBATCH --time=48:00:00
#SBATCH --licenses=cscratch1
#SBATCH --constraint=haswell
#SBATCH --mail-type=begin,end,fail
#SBATCH --mail-user=n.e.sherman@berkeley.edu
#SBATCH --account=m3341

'''
runBase = 'srun -n 1 -c 2 --cpu_bind=cores ./main '

W = 4.0  # roughly width of spectrum
# Ns = [str(x) for x in range(2, 11)]
# nLs = ['40' if i > 5 else str(2**i) for i in range(2, 11)]
Ns = ['32']
nLs=['80']
os.makedirs('inputs/', exist_ok=True)
for N, nL in zip(Ns, nLs):
    label = N + nL
    inputs['N'] = N
    inputs['nLanczos'] = nL
    inputs['etas'] = '0.1' + ',' + str(W / float(N))
    writeInput(inputs, cwd + '/inputs/' + label, table)
    os.system('./main inputs/' + label)


# write and run sbatch script here.
