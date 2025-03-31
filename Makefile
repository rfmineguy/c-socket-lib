SERVER_SOURCES 	:= $(filter-out $(wildcard src/server/*_spec.c), $(wildcard src/server/*.c))
CLIENT_SOURCES 	:= $(wildcard src/client/*.c)
COMMON_SOURCES  := $(wildcard src/*.c)

GENERATED_FILES := src/generated/vector_server_client_handler.c\
									 src/generated/hash_table_msg_type_msg_decoder.c
TPL_OUTDIR  		:= src/generated

CFLAGS := -ggdb -Isrc/server/ -Isrc/generated/ -Isrc/ -Isrc/spec/

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

out/server: $(SERVER_SOURCES) $(COMMON_SOURCES) $(GENERATED_FILES)
	gcc $^ -o $@ $(CFLAGS)

out/client: $(CLIENT_SOURCES) $(COMMON_SOURCES) $(GENERATED_FILES)
	gcc $^ -o $@ $(CFLAGS)

template_gen: always $(GENERATED_FILES)
$(TPL_OUTDIR)/vector_server_client_handler.c $(TPL_OUTDIR)/vector_server_client_handler.h: templates/vector.tpl templates/vector.htpl templates/vector.ctpl
	generics-generator vector --datatype="server_client_handler" --header="serverdata.h" --outputdir=$(TPL_OUTDIR)

$(TPL_OUTDIR)/hash_table_msg_type_msg_decoder.c $(TPL_OUTDIR)/hash_table_msg_type_msg_decoder.h: templates/hash_table.tpl templates/hash_table.htpl templates/hash_table.ctpl
	generics-generator hash_table --key-name="msg_type" --val-name="msg_decoder" --key-type="int" --val-type="msg_decoder" --header="message.h" --outputdir=$(TPL_OUTDIR)
