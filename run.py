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
cores = [' 2 ', ' 4 ', ' 4 ', ' 8 ', ' 8 ', ' 8 ']

baseSlurm = '''#SBATCH --qos=debug
#SBATCH --nodes=8
#SBATCH --time=30:00
#SBATCH --licenses=cscratch1
#SBATCH --constraint=haswell

'''
runFirst = 'srun -N'
runSecond = '-n 1 -c 2 --cpu_bind=cores ./main'
trash = baseSlurm + runFirst + ' 2 ' + runSecond
print (trash)
if not os.path.exists(cwd + '/inputs/'):
    os.mkdir('inputs/')
for N, nL, c in zip(Ns, nLs, cores):
    label = N + nL
    inputs['N'] = N
    inputs['nLanczos'] = nL
    inputs['etas'] = '0.1' + ',' + str(W / float(N))
    writeInput(inputs, cwd + '/inputs/' + label, table)
    baseSlurm += runFirst + c + runSecond + cwd + '/inputs/' + label + '\n'
f = open('run.sh', 'w+')
f.write(baseSlurm)
f.close()

# write and run sbatch script here.
