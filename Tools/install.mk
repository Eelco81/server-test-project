
-include ${ROOT_DIR}/Tools/config.mk

clean-install:
	rm -rf ${INSTALL_DIR}

install: clean-install
	mkdir ${INSTALL_DIR}
	mkdir ${INSTALL_DIR}/Config
	cp ${ROOT_DIR}/Config/* ${INSTALL_DIR}/Config
	cp ${APP_DIR}/Make/server* ${INSTALL_DIR}
	make install -C ${WEB_DIR}
