
ROOT_DIR=./..
-include ${ROOT_DIR}/Tools/config.mk

npm:
	npm install

all:
	npx webpack --config webpack.conf.js

install:
	mkdir $(INSTALL_DIR)/Public
	cp index.html $(INSTALL_DIR)/Public
	cp style.css $(INSTALL_DIR)/Public
	cp main.js $(INSTALL_DIR)/Public