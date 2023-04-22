CC= cc

CFLAGS += -Wall -Wextra -Wpedantic -Waggregate-return
CFLAGS += -Wvla -Wwrite-strings -Wfloat-equal -g3

CPP_FLAGS += _XOPEN_SOURCE

L_XTRA_OBJS +=
D_XTRA_OBJS += obj/parse_args.o obj/socket_server.o obj/socket_client.o
CMN_OBJS = obj/signal_hdlr.o

D_OBJS = obj/dispatcher.o $(D_XTRA_OBJS) $(CMN_OBJS)
L_OBJS = obj/listener.o $(L_XTRA_OBJS) $(CMN_OBJS)

all: dispatcher listener

dispatcher: LDLIBS += -L. -lm -lpthread
dispatcher: $(D_OBJS)
	$(CC) $(CFLAGS) $(D_OBJS) $(LDLIBS) -o $@

listener: LDLIBS += -L. -lm -lpthread
listener: $(L_OBJS)
	$(CC) $(CFLAGS) $(L_OBJS) $(LDLIBS) -o $@

obj/%.o : src/*/%.c
	$(CC) $(CFLAGS) $(LDLIBS) -c $< -o $@

obj/%.o : src/%.c
	$(CC) $(CFLAGS) $(LDLIBS) -c $< -o $@

#obj/%.o : test/%.c
#	$(CC) $(CFLAGS) $(LDLIBS) -c $< -o $@

.PHONY: debug
debug: CFLAGS += -g3
debug: dispatcher listener

CHECK_OBJS += test/test_all.o

.PHONY: check
check: test/test_all
	./test/test_all
test/test_all: CFLAGS += -g3
test/test_all: LDFLAGS += -L.
test/test_all: $(CHECK_OBJS)
test/test_all: obj/common.o
test/test_all: LDLIBS += -lcheck -lm -lrt -lpthread -lsubunit

.PHONY: profile
profile: CFLAGS += -pg
profile: LDFLAGS += -pg
profile: dispatcher listener

.PHONY: clean
clean:
	$(RM) obj/* dispatcher listener test/*.o profile.a test/test_all test/*.o 
	$(RM) src/dispatcher/*.o src/*.o
