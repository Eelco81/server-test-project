
ROOT_DIR=.

-include $(ROOT_DIR)/Tools/external.mk
-include $(ROOT_DIR)/Tools/install.mk

application:
	make all -C App/Make

client:
	make all -C $(WEB_DIR)

test: external
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
	make integration-test -C $(APP_DIR)/Make

clean: 
	make clean -C $(APP_DIR)/Make

distclean: clean clean-external clean-install
