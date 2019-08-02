#SBATCH --qos=debug
#SBATCH --nodes=8
#SBATCH --time=30:00
#SBATCH --licenses=cscratch1
#SBATCH --constraint=haswell

srun -N 2 -n 1 -c 2 --cpu_bind=cores ./main/Users/nsherman/Documents/Research/Joel/spectral/inputs/2040
srun -N 4 -n 1 -c 2 --cpu_bind=cores ./main/Users/nsherman/Documents/Research/Joel/spectral/inputs/4060
srun -N 4 -n 1 -c 2 --cpu_bind=cores ./main/Users/nsherman/Documents/Research/Joel/spectral/inputs/6080
srun -N 8 -n 1 -c 2 --cpu_bind=cores ./main/Users/nsherman/Documents/Research/Joel/spectral/inputs/80100
srun -N 8 -n 1 -c 2 --cpu_bind=cores ./main/Users/nsherman/Documents/Research/Joel/spectral/inputs/90100
srun -N 8 -n 1 -c 2 --cpu_bind=cores ./main/Users/nsherman/Documents/Research/Joel/spectral/inputs/100100
