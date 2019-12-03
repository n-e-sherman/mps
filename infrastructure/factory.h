#ifndef __FACTORY_H_
#define __FACTORY_H_

#include "itensor/all.h"
#include "lattice/latticebuilder.h"
#include "repository/repositorybuilder.h"
#include "model/sitebuilder.h"
#include "model/modelbuilder.h"
#include "state/statebuilder.h"
#include "lanczos/lanczosbuilder.h"
#include "services/spectralbroadening.h"
#include "services/spectralweights.h"
#include "chebyshev/chebyshevbuilder.h"
#include "chebyshev/chebyshevservice.h"
#include "services/moments.h"

/* Memory manage the new pointers. */
LatticeBuilder* latticeFactory() {return new LatticeBuilder(); }
RepositoryBuilder* repositoryFactory() {return new RepositoryBuilder(); }
SiteBuilder* siteFactory() {return new SiteBuilder(); }
ModelBuilder* modelFactory() {return new ModelBuilder(latticeFactory(), siteFactory(), repositoryFactory()); }
StateBuilder* stateFactory() {return new StateBuilder(modelFactory(), latticeFactory(), repositoryFactory()); }
LanczosBuilder* lanczosFactory() {return new LanczosBuilder(modelFactory(), stateFactory(), repositoryFactory()); }
SpectralWeights* spectralWeightsFactory() {return new SpectralWeights(modelFactory(), lanczosFactory(), repositoryFactory()); }
SpectralBroadening* spectralBroadeningFactory() {return new SpectralBroadening(modelFactory(), lanczosFactory(), repositoryFactory()); }
Moments* momentsFactory() {return new Moments(modelFactory(), stateFactory(), latticeFactory(), repositoryFactory()); }

ChebyshevBuilder* chebyshevBuilderFactory() {return new ChebyshevBuilder(modelFactory(), stateFactory(), latticeFactory(), repositoryFactory()); }
ChebyshevService* chebyshevServiceFactory() {return new ChebyshevService(chebyshevBuilderFactory(), repositoryFactory()); }

#endif
