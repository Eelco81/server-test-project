
ROOT_DIR=./..
-include ${ROOT_DIR}/Tools/config.mk

npm:
	npm install

webpack:
	npx webpack --config webpack.conf.js

install:
	rm -rf $(INSTALL_DIR)/Public
	mkdir $(INSTALL_DIR)/Public
	cp index.html $(INSTALL_DIR)/Public
	cp dist/main.js $(INSTALL_DIR)/Public
	cp dist/main.js.map $(INSTALL_DIR)/Public || :

clean:
	rm -rf dist

karma:
	npx karma start karma.conf.js
