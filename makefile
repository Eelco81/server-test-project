
TOOL_DIR=./Tools

-include $(TOOL_DIR)/external.mk

application:
	make all -C App/Make

client:
	make all -C Public

test: external
	make test -C Lib/AppUtils/Make
	make test -C Lib/Core/Make
	make test -C Lib/Http/Make
	make test -C Lib/Math/Make
	make test -C Lib/Router/Make
	make test -C Lib/SimComponents/Make
	make test -C Lib/Simulation/Make
	make test -C Lib/TcpServer/Make
	make test -C Lib/WebSockets/Make

integration-test:
	make integration-test -C App/Make

clean:
	make clean -C App/Make

distclean: clean clean-external