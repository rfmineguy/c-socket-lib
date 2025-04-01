SERVER_SOURCES 	:= $(filter-out src/server/main.c, $(wildcard src/server/*.c))
CLIENT_SOURCES 	:= $(filter-out src/client/main.c, $(wildcard src/client/*.c))
COMMON_SOURCES  := $(wildcard src/*.c)
GENERATED_FILES := src/generated/vector_server_client_handler.c\
									 src/generated/hash_table_client_msg_type_msg_decoder.c\
									 src/generated/hash_table_server_msg_type_msg_decoder.c

SERVER_OBJECTS 	  := $(patsubst src/server/%.c, out/server/%.o, $(SERVER_SOURCES))
CLIENT_OBJECTS 	  := $(patsubst src/client/%.c, out/client/%.o, $(CLIENT_SOURCES))
COMMON_OBJECTS    := $(patsubst src/%.c, out/common/%.o, $(COMMON_SOURCES))
GENERATED_OBJECTS := $(patsubst src/generated/%.c, out/generated/%.o, $(GENERATED_FILES))


TPL_OUTDIR  		:= src/generated

CFLAGS := -Isrc/server/ -Isrc/generated/ -Isrc/ -Isrc/spec/

export GEN_TEMPLATE_PATH=templates/

.PHONY: always clean
.PHONY: build-all build-server build-client

always:
	@echo "server : $(SERVER_OBJECTS)"
	@echo "client : $(CLIENT_OBJECTS)"
	@echo "common : $(COMMON_OBJECTS)"
	mkdir -p out/client
	mkdir -p out/server
	mkdir -p out/common
	mkdir -p out/generated
	mkdir -p out/lib
	mkdir -p out/bin
	mkdir -p $(TPL_OUTDIR)

clean:
	rm -r out
clean-templates:
	rm -r $(TPL_OUTDIR)

build-all: template_gen build-server build-client
build-server: always out/bin/server
build-client: always out/bin/client
build-objects: always $(SERVER_OBJECTS) $(CLIENT_OBJECTS) $(COMMON_OBJECTS) $(GENERATED_OBJECTS)
build-static-lib: always out/lib/libsocketclient.a out/lib/libsocketserver.a

out/bin/server: out/lib/libsocketserver.a
	gcc src/server/main.c -o $@ $(CFLAGS) -Lout/lib/ -lsocketserver

out/bin/client: out/lib/libsocketclient.a
	gcc src/client/main.c -o $@ $(CFLAGS) -Lout/lib/ -lsocketclient

out/client/%.o: src/client/%.c
	gcc -c $^ -o $@ $(CFLAGS)

out/server/%.o: src/server/%.c
	gcc -c $^ -o $@ $(CFLAGS)

out/common/%.o: src/%.c
	gcc -c $^ -o $@ $(CFLAGS)

out/generated/%.o: src/generated/%.c
	gcc -c $^ -o $@ $(CFLAGS)

out/lib/libsocketserver.a: $(SERVER_OBJECTS) $(COMMON_OBJECTS) $(GENERATED_OBJECTS)
	ar -rc $@ $^

out/lib/libsocketclient.a: $(CLIENT_OBJECTS) $(COMMON_OBJECTS) $(GENERATED_OBJECTS)
	ar -rc $@ $^

template_gen: always $(GENERATED_FILES)
$(TPL_OUTDIR)/vector_server_client_handler.c $(TPL_OUTDIR)/vector_server_client_handler.h: templates/vector.tpl templates/vector.htpl templates/vector.ctpl
	generics-generator vector --datatype="server_client_handler" --header="serverdata.h" --outputdir=$(TPL_OUTDIR)

$(TPL_OUTDIR)/hash_table_client_msg_type_msg_decoder.c $(TPL_OUTDIR)/hash_table_client_msg_type_msg_decoder.h: templates/hash_table.tpl templates/hash_table.htpl templates/hash_table.ctpl
	generics-generator hash_table --key-name="client_msg_type" --val-name="msg_decoder" --key-type="int" --val-type="client_msg_decoder" --header="message.h" --outputdir=$(TPL_OUTDIR)

$(TPL_OUTDIR)/hash_table_server_msg_type_msg_decoder.c $(TPL_OUTDIR)/hash_table_server_msg_type_msg_decoder.h: templates/hash_table.tpl templates/hash_table.htpl templates/hash_table.ctpl
	generics-generator hash_table --key-name="server_msg_type" --val-name="msg_decoder" --key-type="int" --val-type="server_msg_decoder" --header="message.h" --outputdir=$(TPL_OUTDIR)
