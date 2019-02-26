
-include ${ROOT_DIR}/Tools/config.mk

clean-install:
	rm -rf ${INSTALL_DIR}

install: clean-install
	mkdir ${INSTALL_DIR}
	mkdir ${INSTALL_DIR}/Config
	cp ${ROOT_DIR}/Config/* ${INSTALL_DIR}/Config
	cp ${APP_DIR}/Server/Make/server* ${INSTALL_DIR}
	cp ${APP_DIR}/Client/Make/client* ${INSTALL_DIR}
	cp ${APP_DIR}/TestApp/Make/testapp* ${INSTALL_DIR}
	make install -C ${WEB_DIR}
