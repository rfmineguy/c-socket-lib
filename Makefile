SERVER_SOURCES := $(filter-out $(wildcard src/server/*_spec.c), $(wildcard src/server/*.c))
CLIENT_SOURCES := $(wildcard src/client/*.c)

GENERATED_FILES := src/generated/vector_server_client_handler.c
TPL_OUTDIR := src/generated

CFLAGS := -ggdb -Isrc/server/ -I src/generated

export GEN_TEMPLATE_PATH=templates/

.PHONY: always clean
.PHONY: build-all build-server build-client

always:
	@echo "$(SERVER_SOURCES)"
	mkdir -p out
	mkdir -p $(TPL_OUTDIR)

clean:
	rm -r out
	rm -r $(TPL_OUTDIR)

build-all: template_gen build-server build-client
build-server: always out/server
build-client: always out/client

out/server: $(SERVER_SOURCES) $(GENERATED_FILES)
	gcc $^ -o $@ $(CFLAGS)

out/client: $(CLIENT_SOURCES) $(GENERATED_FILES)
	gcc $^ -o $@ $(CFLAGS)

template_gen: always $(GENERATED_FILES)
$(TPL_OUTDIR)/vector_server_client_handler.c $(TPL_OUTDIR)/vector_server_client_handler.h: templates/vector.tpl templates/vector.htpl templates/vector.ctpl
	generics-generator vector --datatype="server_client_handler" --header="serverdata.h" --outputdir=$(TPL_OUTDIR)
