DEMODIR = $(GFXDEMO)
GFXINC +=   $(DEMODIR)
GFXSRC +=	$(DEMODIR)/main.c \
			$(DEMODIR)/gui.c \
			$(DEMODIR)/can_id_transformer.c \
            $(DEMODIR)/dialWidget/dial.c \
            $(DEMODIR)/cornerWidget/corner.c \
			$(DEMODIR)/qspiFlash.c	\
            $(DEMODIR)/fileLayout.c	\
 			$(DEMODIR)/fuelGauge/fuelGauge.c	
