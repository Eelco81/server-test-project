
-include ${ROOT_DIR}/Tools/config.mk

pip:
	make pip -C ${APP_DIR}/Make

npm:
	make npm -C ${WEB_DIR}

download: clean-external
	mkdir ${DOWNLOAD_DIR}
	wget -P ${DOWNLOAD_DIR} https://github.com/google/googlemock/archive/release-1.7.0.zip
	mv ${DOWNLOAD_DIR}/release-1.7.0.zip ${DOWNLOAD_DIR}/gmock-release-1.7.0.zip
	wget -P ${DOWNLOAD_DIR} https://github.com/google/googletest/archive/release-1.7.0.zip
	mv ${DOWNLOAD_DIR}/release-1.7.0.zip ${DOWNLOAD_DIR}/gtest-release-1.7.0.zip
	wget -P ${DOWNLOAD_DIR} https://raw.githubusercontent.com/983/SHA1/master/sha1.hpp
	wget -P ${DOWNLOAD_DIR} https://github.com/nlohmann/json/releases/download/v3.3.0/json.hpp

external: pip npm download
	mkdir ${EXTERNAL_DIR}
	mkdir ${EXTERNAL_DIR}/gmock
	mkdir ${EXTERNAL_DIR}/gmock/googlemock
	mkdir ${EXTERNAL_DIR}/gmock/googlemock/gtest
	unzip ${DOWNLOAD_DIR}/gmock-release-1.7.0.zip -d ${EXTERNAL_DIR}
	mv ${EXTERNAL_DIR}/googlemock-release-1.7.0/* ${EXTERNAL_DIR}/gmock/googlemock
	rm -rf ${EXTERNAL_DIR}/googlemock-release-1.7.0
	unzip ${DOWNLOAD_DIR}/gtest-release-1.7.0.zip -d ${EXTERNAL_DIR}
	mv ${EXTERNAL_DIR}/googletest-release-1.7.0/* ${EXTERNAL_DIR}/gmock/googlemock/gtest
	rm -rf ${EXTERNAL_DIR}/googletest-release-1.7.0
	mkdir ${EXTERNAL_DIR}/sha1
	cp ${DOWNLOAD_DIR}/sha1.hpp ${EXTERNAL_DIR}/sha1
	mkdir ${EXTERNAL_DIR}/nlohman
	cp ${DOWNLOAD_DIR}/json.hpp ${EXTERNAL_DIR}/nlohman

clean-external:
	rm -rf ${DOWNLOAD_DIR}
	rm -rf ${EXTERNAL_DIR}
	rm -rf ${WEB_DIR}/node_modules

