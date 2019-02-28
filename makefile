
ROOT_DIR=.

-include $(ROOT_DIR)/Tools/external.mk
-include $(ROOT_DIR)/Tools/install.mk

.PHONY: applicatiom all web test clean

application:
	make all -C $(APP_DIR)/Server/Make
	make all -C $(APP_DIR)/Client/Make
	make all -C $(APP_DIR)/TestApp/Make

web:
	make webpack -C $(WEB_DIR)

test:
	make test -C $(LIB_DIR)/AppUtils/Make
	make test -C $(LIB_DIR)/Core/Make
	make test -C $(LIB_DIR)/Http/Make
	make test -C $(LIB_DIR)/Math/Make
	make test -C $(LIB_DIR)/Router/Make
	make test -C $(LIB_DIR)/SimComponents/Make
	make test -C $(LIB_DIR)/Simulation/Make
	make test -C $(LIB_DIR)/TcpServer/Make
	make test -C $(LIB_DIR)/WebSockets/Make

integration-test:
	make integration-test -C $(TEST_DIR)

clean: 
	make clean -C $(APP_DIR)/Server/Make
	make clean -C $(APP_DIR)/Client/Make
	make clean -C $(APP_DIR)/TestApp/Make

distclean: clean clean-external clean-install

all: application install
