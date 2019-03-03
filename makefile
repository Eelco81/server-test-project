
ROOT_DIR=.

-include $(ROOT_DIR)/Tools/external.mk
-include $(ROOT_DIR)/Tools/install.mk

.PHONY: web test clean

%-all:
	$(eval APPS:=$(dir $(wildcard $(APP_DIR)/*/.)))
	for app in $(APPS) ; do \
		$(MAKE) $@ -C $$app/Make ; \
	done

web:
	$(MAKE) webpack -C $(WEB_DIR)

test:
	$(eval LIBS:=$(dir $(wildcard $(LIB_DIR)/*/.)))
	for lib in $(LIBS) ; do \
		$(MAKE) $@ -C $$lib/Make ; \
	done

coverage:
	$(eval LIBS:=$(dir $(wildcard $(LIB_DIR)/*/.)))
	for lib in $(LIBS) ; do \
		$(MAKE) $@ -C $$lib/Make ; \
	done

integration-test:
	$(MAKE) $@ -C $(TEST_DIR)

clean: 
	$(eval APPS:=$(dir $(wildcard $(APP_DIR)/*/.)))
	for app in $(APPS) ; do \
		$(MAKE) $@ -C $$app/Make ; \
	done
	$(MAKE) $@ -C $(TEST_DIR)

distclean: clean clean-external clean-install
