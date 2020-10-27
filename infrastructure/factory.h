#ifndef __FACTORY_H_
#define __FACTORY_H_

#include "itensor/all.h"
#include "lattice/latticebuilder.h"
#include "repository/repositorybuilder.h"
#include "model/sitebuilder.h"
#include "model/modelbuilder.h"
#include "state/statebuilder.h"
#include "sweeper/sweeperbuilder.h"
#include "evolver/evolverbuilder.h"

#include "chebyshev/chebyshevbuilder.h"
#include "chebyshev/chebyshevservice.h"
#include "correlation/correlationbuilder.h"
#include "correlation/correlationservice.h"
#include "measurement/measurementbuilder.h"
#include "measurement/measurementservice.h"


/* Memory manage the new pointers. */
LatticeBuilder* latticeFactory() { return new LatticeBuilder(); }
RepositoryBuilder* repositoryFactory() { return new RepositoryBuilder(); }
SiteBuilder* siteFactory() { return new SiteBuilder(); }
ModelBuilder* modelFactory() { return new ModelBuilder(latticeFactory(), siteFactory(), repositoryFactory()); }
OperatorBuilder* operatorFactory() { return new OperatorBuilder(modelFactory(), latticeFactory(), repositoryFactory()); }
SweeperBuilder* sweeperFactory() { return new SweeperBuilder(); }
EvolverBuilder* evolverFactory() { return new EvolverBuilder(modelFactory()); }
StateBuilder* stateFactory() { return new StateBuilder(modelFactory(), latticeFactory(), repositoryFactory(), evolverFactory()); }
MeasurementBuilder* measurementFactory() { return new MeasurementBuilder(modelFactory(), latticeFactory(), operatorFactory(), repositoryFactory()); }

ChebyshevBuilder* chebyshevBuilderFactory() { return new ChebyshevBuilder(modelFactory(), stateFactory(), latticeFactory(), repositoryFactory(), sweeperFactory(), measurementFactory(), operatorFactory()); }
ChebyshevService* chebyshevServiceFactory() { return new ChebyshevService(chebyshevBuilderFactory(), repositoryFactory()); }

CorrelationBuilder* correlationBuilderFactory() { return new CorrelationBuilder(stateFactory(), evolverFactory(), modelFactory(), repositoryFactory(), measurementFactory(), operatorFactory()); }
CorrelationService* correlationServiceFactory() { return new CorrelationService(correlationBuilderFactory(), repositoryFactory()); }

#endif
