
-include ${ROOT_DIR}/Tools/config.mk

clean-install:
	rm -rf ${INSTALL_DIR}

install: clean-install
	mkdir ${INSTALL_DIR}
