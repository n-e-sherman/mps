# 1. Put this file in the same folder as your 'driver' code 
#    (the code containing the 'main' function).

# 2. Edit LIBRARY_DIR to point at the location of your ITensor Library
#    source folder (this is the folder that has options.mk in it)
LIBRARY_DIR=$(HOME)/itensor

# 3. If your 'main' function is in a file called 'myappname.cc', then
#    set APP to 'myappname'. Running 'make' will compile the app.
#    Running 'make debug' will make a program called 'myappname-g'
#    which includes debugging symbols and can be used in gdb (Gnu debugger);
APP= test

# 4. Add any headers your program depends on here. The make program
#    will auto-detect if these headers have changed and recompile your app.
INF_HEADERS= infrastructure/builder.h infrastructure/cache.h infrastructure/calculator.h infrastructure/factory.h infrastructure/service.h infrastructure/util.h infrastructure/validator.h 
LAT_HEADERS= lattice/chain.h lattice/lattice.h lattice/latticebuilder.h lattice/thermalchain.h 
MOD_HEADERS= model/heisenberg.h model/model.h model/modelbuilder.h model/sitebuilder.h model/heisenberglouiville.h
QTY_HEADERS= quantity/quantitycalculatorbuilder.h
REP_HEADERS= repository/repository.h repository/repositorybuilder.h
STT_HEADERS= state/groundstate.h state/spectralstate.h state/statecalculator.h state/thermalstate.h state/state.h
#chain.h factory.h groundstatecalculator.h heisenberg.h lattice.h latticebuilder.h model.h modelbuilder.h service.h sitebuilder.h thermalchain.h validator.h cache.h repository.h repositorybuilder.h
# 5. For any additional .cc (source) files making up your project,
#    add their full filenames here.
CCFILES=$(APP).cc
CCCOMCRAY=g++-9 -std=c++17 -fPIC
#################################################################
#################################################################
#################################################################
#################################################################


include $(LIBRARY_DIR)/this_dir.mk
include $(LIBRARY_DIR)/options.mk

TENSOR_HEADERS=$(LIBRARY_DIR)/itensor/core.h

#Mappings --------------
OBJECTS=$(patsubst %.cc,%.o, $(CCFILES))
GOBJECTS=$(patsubst %,.debug_objs/%, $(OBJECTS))

#Rules ------------------
%.o: %.cc $(INF_HEADERS) $(LAT_HEADERS) $(MOD_HEADERS) $(QTY_HEADERS) $(REP_HEADERS) $(TENSOR_HEADERS)
	$(CCCOM) -c $(CCFLAGS) -o $@ $<

.debug_objs/%.o: %.cc $(INF_HEADERS) $(LAT_HEADERS) $(MOD_HEADERS) $(QTY_HEADERS) $(REP_HEADERS) $(TENSOR_HEADERS)
	$(CCCOM) -c $(CCGFLAGS) -o $@ $<

#Targets -----------------

build: $(APP)
debug: $(APP)-g

$(APP): $(OBJECTS) $(ITENSOR_LIBS)
	$(CCCOMCRAY) $(CCFLAGS) $(OBJECTS) -o $(APP) $(LIBFLAGS)

$(APP)-g: mkdebugdir $(GOBJECTS) $(ITENSOR_GLIBS)
	$(CCCOMCRAY) $(CCGFLAGS) $(GOBJECTS) -o $(APP)-g $(LIBGFLAGS)

clean:
	rm -fr .debug_objs *.o $(APP) $(APP)-g

mkdebugdir:
	mkdir -p .debug_objs

