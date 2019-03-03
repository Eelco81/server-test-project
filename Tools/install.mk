
-include ${ROOT_DIR}/Tools/config.mk

clean-install:
	rm -rf ${INSTALL_DIR}

install: clean-install
	mkdir ${INSTALL_DIR}
	mkdir ${INSTALL_DIR}/Config
	cp ${ROOT_DIR}/Config/* ${INSTALL_DIR}/Config
	$(eval APPS:=$(dir $(wildcard $(APP_DIR)/*/.)))
	for app in $(APPS) ; do \
		$(MAKE) $@ -C $$app/Make ; \
	done
	$(MAKE) $@ -C ${WEB_DIR}
