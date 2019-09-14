#include "itensor/all.h"
#include "lattice/latticebuilder.h"
#include "model/modelbuilder.h"
#include "model/sitebuilder.h"
#include "state/statecalculator.h"
#include "quantity/quantitycalculatorbuilder.h"
#include "repository/repositorybuilder.h"

/* Memory manage the new pointers. */

LatticeBuilder* latticeFactory() {return new LatticeBuilder(); }
ModelBuilder* modelFactory() {return new ModelBuilder(latticeFactory(), siteFactory()); }
SiteBuilder* siteFactory() {return new SiteBuilder(); }
// GroundStateCalculator* groundStateFactory() {return new GroundStateCalculator(modelFactory(), repositoryFactory()); }
StateCalculator* stateFactory() {return new StateCalculator(modelFactory(), repositoryFactory()); }
QuantityCalculator* quantityFactory() {return new QuantityCalculator(modelFactory(), stateFactory(), repositoryFactory()); }
// SpectralCalculator* spectralFactory() {return new SpectralStateCalculator(latticeFactory(), StateCalculator(), repositoryFactory(), ); }
RepositoryBuilder* repositoryFactory() {return new RepositoryBuilder(); }

