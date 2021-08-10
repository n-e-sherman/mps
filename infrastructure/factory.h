#ifndef __FACTORY_H_
#define __FACTORY_H_

#include "itensor/all.h"

#include "kzm/kzmbuilder.h"
#include "kzm/kzmservice.h"
#include "measurement/measurementbuilder.h"
#include "correlation/correlationservice.h"
#include "correlation/correlationbuilder.h"
#include "chebyshev/chebyshevservice.h"
#include "chebyshev/chebyshevbuilder.h"
#include "static/staticbuilder.h"
#include "static/magnetizationservice.h"
#include "static/staticcorrelationservice.h"

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
ModelBuilder* modelFactory() { return new ModelBuilder( latticeFactory(), sitesFactory() ); }
OperatorBuilder* operatorFactory() { return new OperatorBuilder( sitesFactory(), latticeFactory() ); }
SweeperBuilder* sweeperFactory() { return new SweeperBuilder(); }
EvolverBuilder* evolverFactory() { return new EvolverBuilder( modelFactory() ); }
StateBuilder* stateFactory() { return new StateBuilder( modelFactory(), latticeFactory(), evolverFactory() ); }
MeasurementBuilder* measurementFactory() { return new MeasurementBuilder( sitesFactory(), latticeFactory(), operatorFactory() ); }
ChebyshevBuilder* chebyshevFactory() { return new ChebyshevBuilder( measurementFactory(), modelFactory(), stateFactory(), operatorFactory(), repositoryFactory() ); }
CorrelationBuilder* correlationFactory() { return new CorrelationBuilder( evolverFactory(), measurementFactory(), stateFactory(), operatorFactory(), repositoryFactory() ); }
KZMBuilder* kzmFactory() { return new KZMBuilder( repositoryFactory() ); }
StaticBuilder* staticFactory() {return new StaticBuilder( stateFactory(), measurementFactory(), operatorFactory(), latticeFactory(),repositoryFactory() ); }

/* Services */
ChebyshevService* chebyshevServiceFactory() { return new ChebyshevService( chebyshevFactory(), repositoryFactory() ); }
CorrelationService* correlationServiceFactory() { return new CorrelationService( correlationFactory(), repositoryFactory() ); }
KZMService* kzmServiceFactory() { return new KZMService( kzmFactory(), repositoryFactory() ); }
MagnetizationService* magnetizationServiceFactory() { return new MagnetizationService( staticFactory(), repositoryFactory() ); }
StaticCorrelationService* staticCorrelationServiceFactory() { return new StaticCorrelationService( staticFactory(), repositoryFactory() ); }

#endif
