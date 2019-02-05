# create an executable

ROOT_DIR        = ./../..
SOURCE_DIR      = ./../Src
DEPENDENCIES    = Websockets Router Http TcpServer SimComponents Simulation AppUtils Core
TARGET          = server

-include $(ROOT_DIR)/Tools/build.mk # DO NOT DELETE

pip: 
	pip install -r ../Test/requirements.txt

integration-test : 
	rm *.xml || :
	cd ../Test; behave --junit --junit-directory ../Make