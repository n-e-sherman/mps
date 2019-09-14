#include "itensor/all.h"
#include "lattice/latticebuilder.h"
#include "repository/repositorybuilder.h"
#include "model/sitebuilder.h"
#include "model/modelbuilder.h"
#include "state/statebuilder.h"
#include "krylov/krylovbuilder.h"
#include "services/spectralbroadening.h"
#include "services/spectralweights.h"

/* Memory manage the new pointers. */
LatticeBuilder* latticeFactory() {return new LatticeBuilder(); }
RepositoryBuilder* repositoryFactory() {return new RepositoryBuilder(); }
SiteBuilder* siteFactory() {return new SiteBuilder(); }
ModelBuilder* modelFactory() {return new ModelBuilder(latticeFactory(), siteFactory()), repositoryFactory(); }
StateBuilder* stateFactory() {return new StateBuilder(modelFactory(), latticeFactory(), repositoryFactory()); }
KrylovBuilder* krylovFactory() {return new KrylovBuilder(modelFactory(), stateFactory(), repositoryFactory()); }
SpectralWeights* SpectralWeightsFactory() {return new SpectralWeights(modelFactory(), krylovFactory(), repositoryFactory()); }
SpectralWeights* SpectralWeightsFactory() {return new SpectralWeights(modelFactory(), krylovFactory(), repositoryFactory()); }

