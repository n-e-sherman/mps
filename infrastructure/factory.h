#ifndef __FACTORY_H_
#define __FACTORY_H_

#include "itensor/all.h"

#include "measurement/measurementbuilder.h"
#include "correlation/correlationservice.h"
#include "correlation/correlationbuilder.h"
#include "chebyshev/chebyshevservice.h"
#include "chebyshev/chebyshevbuilder.h"

#include "state/statebuilder.h"
#include "evolver/evolverbuilder.h"
#include "sweeper/sweeperbuilder.h"
#include "operator/operatorbuilder.h"
#include "model/modelbuilder.h"
#include "sites/sitesbuilder.h"
#include "lattice/latticebuilder.h"
#include "repository/repositorybuilder.h"

/* Objects */
RepositoryBuilder* repositoryFactory() { return new RepositoryBuilder(); }
LatticeBuilder* latticeFactory() { return new LatticeBuilder(); }
SitesBuilder* sitesFactory() { return new SitesBuilder(); }
ModelBuilder* modelFactory() { return new ModelBuilder(latticeFactory(), sitesFactory()); }
OperatorBuilder* operatorFactory() { return new OperatorBuilder(sitesFactory(), latticeFactory()); }
SweeperBuilder* sweeperFactory() { return new SweeperBuilder(); }
EvolverBuilder* evolverFactory() { return new EvolverBuilder(modelFactory()); }
StateBuilder* stateFactory() { return new StateBuilder(modelFactory(), latticeFactory(), evolverFactory()); }
MeasurementBuilder* measurementFactory() { return new MeasurementBuilder(sitesFactory(), latticeFactory(), operatorFactory()); }

/* Services */
ChebyshevBuilder* chebyshevBuilderFactory() { return new ChebyshevBuilder(measurementFactory(), modelFactory(), stateFactory(), operatorFactory(), repositoryFactory()); }
ChebyshevService* chebyshevServiceFactory() { return new ChebyshevService(chebyshevBuilderFactory(), repositoryFactory()); }
CorrelationBuilder* correlationBuilderFactory() { return new CorrelationBuilder(evolverFactory(), measurementFactory(), stateFactory(), operatorFactory(), repositoryFactory()); }
CorrelationService* correlationServiceFactory() { return new CorrelationService(correlationBuilderFactory(), repositoryFactory()); }

#endif
