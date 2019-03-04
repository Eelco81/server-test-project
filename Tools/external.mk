
-include ${ROOT_DIR}/Tools/config.mk

GMOCK_VERSION   = 1.7.0
GMOCK_LIB       = googlemock-release-${GMOCK_VERSION}
GTEST_LIB       = googletest-release-${GMOCK_VERSION}
GMOCK_DIR       = $(EXTERNAL_DIR)/$(GMOCK_LIB)
GTEST_DIR       = $(EXTERNAL_DIR)/$(GTEST_LIB)

pip:
	$(MAKE) $@ -C ${TEST_DIR}

npm:
	$(MAKE) $@ -C ${WEB_DIR}

gtest:
	mkdir ${DOWNLOAD_DIR} || :
	wget -P ${DOWNLOAD_DIR} https://github.com/google/googlemock/archive/release-${GMOCK_VERSION}.zip
	mv ${DOWNLOAD_DIR}/release-${GMOCK_VERSION}.zip ${DOWNLOAD_DIR}/${GMOCK_LIB}.zip
	wget -P ${DOWNLOAD_DIR} https://github.com/google/googletest/archive/release-${GMOCK_VERSION}.zip
	mv ${DOWNLOAD_DIR}/release-${GMOCK_VERSION}.zip ${DOWNLOAD_DIR}/${GTEST_LIB}.zip
	mkdir ${EXTERNAL_DIR} || :
	unzip ${DOWNLOAD_DIR}/${GMOCK_LIB}.zip -d ${EXTERNAL_DIR}
	unzip ${DOWNLOAD_DIR}/${GTEST_LIB}.zip -d ${EXTERNAL_DIR}

json:
	mkdir ${DOWNLOAD_DIR} || :
	wget -P ${DOWNLOAD_DIR} https://github.com/nlohmann/json/releases/download/v3.3.0/json.hpp
	mkdir ${EXTERNAL_DIR} || :
	rm -rf ${EXTERNAL_DIR}/nlohman || :
	mkdir ${EXTERNAL_DIR}/nlohman
	cp ${DOWNLOAD_DIR}/json.hpp ${EXTERNAL_DIR}/nlohman

sha1:
	mkdir ${DOWNLOAD_DIR} || :
	wget -P ${DOWNLOAD_DIR} https://raw.githubusercontent.com/983/SHA1/master/sha1.hpp
	mkdir ${EXTERNAL_DIR} || :
	rm -rf ${EXTERNAL_DIR}/sha1 || :
	mkdir ${EXTERNAL_DIR}/sha1
	cp ${DOWNLOAD_DIR}/sha1.hpp ${EXTERNAL_DIR}/sha1

external: pip npm json sha1 gtest download

clean-external:
	rm -rf ${DOWNLOAD_DIR}
	rm -rf ${EXTERNAL_DIR}
	rm -rf ${WEB_DIR}/node_modules

