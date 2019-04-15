
-include ${ROOT_DIR}/Tools/config.mk

GTEST_VERSION   = 1.8.1
GTEST_LIB       = googletest-release-${GTEST_VERSION}
GMOCK_DIR       = $(EXTERNAL_DIR)/$(GTEST_LIB)/googlemock
GTEST_DIR       = $(EXTERNAL_DIR)/$(GTEST_LIB)/googletest

pip:
	$(MAKE) $@ -C ${TEST_DIR}

npm:
	$(MAKE) $@ -C ${WEB_DIR}

gtest: setup-external
	wget -P ${DOWNLOAD_DIR} https://github.com/google/googletest/archive/release-${GTEST_VERSION}.zip
	mv ${DOWNLOAD_DIR}/release-${GTEST_VERSION}.zip ${DOWNLOAD_DIR}/${GTEST_LIB}.zip
	unzip -u ${DOWNLOAD_DIR}/${GTEST_LIB}.zip -d ${EXTERNAL_DIR}
	cp -r ${GMOCK_DIR}/include/gmock ${EXTERNAL_DIR}/include
	cp -r ${GTEST_DIR}/include/gtest ${EXTERNAL_DIR}/include

json: setup-external
	wget -P ${DOWNLOAD_DIR} https://github.com/nlohmann/json/releases/download/v3.3.0/json.hpp
	cp ${DOWNLOAD_DIR}/json.hpp ${EXTERNAL_DIR}/include

sha1: setup-external
	wget -P ${DOWNLOAD_DIR} https://raw.githubusercontent.com/983/SHA1/master/sha1.hpp
	cp ${DOWNLOAD_DIR}/sha1.hpp ${EXTERNAL_DIR}/include

external: pip npm json sha1 gtest

setup-external:
	mkdir ${DOWNLOAD_DIR} || :
	mkdir ${EXTERNAL_DIR} || :
	mkdir ${EXTERNAL_DIR}/include || :

clean-external:
	rm -rf ${DOWNLOAD_DIR}
	rm -rf ${EXTERNAL_DIR}
	rm -rf ${WEB_DIR}/node_modules

