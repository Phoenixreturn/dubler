DEMODIR = $(GFXDEMO)
GFXINC +=   $(DEMODIR) \
			$(DEMODIR)\modbus_protocol_tcp \
			${LWINC}
GFXSRC +=	$(DEMODIR)/main.c \
			$(DEMODIR)/gui.c \
			$(DEMODIR)/can_id_transformer.c \
            $(DEMODIR)/dialWidget/dial.c \
            $(DEMODIR)/cornerWidget/corner.c \
			$(DEMODIR)/qspiFlash.c	\
            $(DEMODIR)/fileLayout.c	\
 			$(DEMODIR)/fuelGauge/fuelGauge.c \
 			$(DEMODIR)/web/web.c \
 			${LWSRC} \
 			$(DEMODIR)/modbus_protocol_tcp/modbus.c \
 			$(DEMODIR)/modbus_protocol_tcp/modbus-data.c \
 			$(DEMODIR)/modbus_protocol_tcp/modbus-tcp.c \
 			$(DEMODIR)/modbus_thread\modbus_thread.c
