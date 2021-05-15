# 1. Put this file in the same folder as your 'driver' code 
#    (the code containing the 'main' function).

# 2. Edit LIBRARY_DIR to point at the location of your ITensor Library
#    source folder (this is the folder that has options.mk in it)
LIBRARY_DIR=$(HOME)/itensor

# 3. If your 'main' function is in a file called 'myappname.cc', then
#    set APP to 'myappname'. Running 'make' will compile the app.
#    Running 'make debug' will make a program called 'myappname-g'
#    which includes debugging symbols and can be used in gdb (Gnu debugger);
APP = main

# 4. Add any headers your program depends on here. The make program
#    will auto-detect if these headers have changed and recompile your app.
ADD_HEADERS= addons/itensor-basisextension.h addons/itensor-tdvp.h addons/itensor-itdvp.h addons/itensor-itdvp-util.h
INF_HEADERS= infrastructure/cache.h infrastructure/factory.h infrastructure/util.h infrastructure/defaults.h
LAT_HEADERS= lattice/chain.h lattice/lattice.h lattice/latticebuilder.h lattice/thermalchain.h lattice/triangular.h 
MOD_HEADERS= model/heisenberg.h model/model.h model/modelbuilder.h model/xx.h model/xxz.h model/dimer.h model/heisenbergfield.h model/heisenberglr.h
REP_HEADERS= repository/repository.h repository/repositorybuilder.h
SRV_HEADERS= service/service.h
STT_HEADERS= state/groundstate.h state/state.h state/statebuilder.h state/thermalstate.h
SIT_HEADERS= sites/sites.h sites/sitesbuilder.h
CHB_HEADERS= chebyshev/chebyshev.h chebyshev/chebyshevbuilder.h chebyshev/chebyshevservice.h
COR_HEADERS= correlation/correlation.h correlation/correlationbuilder.h correlation/correlationservice.h
SWP_HEADERS= sweeper/sweeper.h sweeper/sweeperbuilder.h sweeper/krylov.h sweeper/projection.h sweeper/exact.h sweeper/identity.h
OPR_HEADERS= operator/operator.h operator/momentum.h operator/position.h operator/operatorbuilder.h
EVR_HEADERS= evolver/evolver.h evolver/evolverbuilder.h evolver/expmpo.h evolver/trotter.h evolver/tdvp.h evolver/trivial.h
MSR_HEADERS= measurement/measurement.h measurement/kspace.h measurement/realspace.h measurement/measurementbuilder.h measurement/local.h
STC_HEADERS= static/magnetization.h static/static.h static/staticbuilder.h static/magnetizationservice.h
KZM_HEADERS= kzm/kzm.h kzm/kzmbuilder.h kzm/kzmservice.h

#LAN_HEADERS= lanczos/lanczos.h lanczos/lanczosbuilder.h lanczos/reorthogonalize.h
#chain.h factory.h groundstatecalculator.h heisenberg.h lattice.h latticebuilder.h model.h modelbuilder.h service.h sitebuilder.h thermalchain.h validator.h cache.h repository.h repositorybuilder.h
# 5. For any additional .cc (source) files making up your project,
#    add their full filenames here.
CCFILES=$(APP).cc
# CCCOMCRAY = CC -std=c++17 -fPIC
#################################################################
#################################################################
#################################################################
#################################################################


include $(LIBRARY_DIR)/this_dir.mk
include $(LIBRARY_DIR)/options.mk

TENSOR_HEADERS=$(LIBRARY_DIR)/itensor/core.h $(LIBRARY_DIR)/itensor/all.h 

#Mappings --------------
OBJECTS=$(patsubst %.cc,%.o, $(CCFILES))
GOBJECTS=$(patsubst %,.debug_objs/%, $(OBJECTS))

#Rules ------------------
%.o: %.cc $(ADD_HEADERS) $(INF_HEADERS) $(LAT_HEADERS) $(MOD_HEADERS) $(REP_HEADERS) $(SRV_HEADERS) $(STT_HEADERS) $(SIT_HEADERS) $(CHB_HEADERS) $(COR_HEADERS) $(SWP_HEADERS) $(OPR_HEADERS) $(EVR_HEADERS) $(MSR_HEADERS) $(STC_HEADERS) $(KZM_HEADERS) $(TENSOR_HEADERS)
	$(CCCOM) -c $(CCFLAGS) -o $@ $<

.debug_objs/%.o: %.cc $(ADD_HEADERS) $(INF_HEADERS) $(LAT_HEADERS) $(MOD_HEADERS) $(REP_HEADERS) $(SRV_HEADERS) $(STT_HEADERS) $(SIT_HEADERS) $(CHB_HEADERS) $(COR_HEADERS) $(SWP_HEADERS) $(OPR_HEADERS) $(EVR_HEADERS) $(MSR_HEADERS) $(STC_HEADERS) $(KZM_HEADERS) $(TENSOR_HEADERS)
	$(CCCOM) -c $(CCGFLAGS) -o $@ $<

#Targets -----------------

build: $(APP)
debug: $(APP)-g

# CCCOMCRAY HERE
$(APP): $(OBJECTS) $(ITENSOR_LIBS)
	$(CCCOM) $(CCFLAGS) $(OBJECTS) -o $(APP) $(LIBFLAGS)

$(APP)-g: mkdebugdir $(GOBJECTS) $(ITENSOR_GLIBS)
	$(CCCOM) $(CCGFLAGS) $(GOBJECTS) -o $(APP)-g $(LIBGFLAGS)

clean:
	rm -fr .debug_objs *.o $(APP) $(APP)-g

mkdebugdir:
	mkdir -p .debug_objs

