CC ?= gcc
CFLAGS = -std=gnu99 -O0 -Wall -Wextra

EXEC = check_execute \
	   main naive_transpose \
	   sse_transpose sse_prefetch_transpose \
	   avx_transpose avx_prefetch_transpose \
	   verify

OUTPUT_DIR := executes

GIT_HOOKS := .git/hooks/applied

.PHONY: all
all: $(EXEC)

$(GIT_HOOKS):
	@scripts/install-git-hooks
	@echo

SRCS_common = main.c

main: $(SRCS_common)
	$(CC) $(CFLAGS) -msse -mavx \
		-DMAIN=$@ -o $(OUTPUT_DIR)/$@ \
		$(SRCS_common)

naive_transpose: $(SRCS_common)
	$(CC) $(CFLAGS) -mavx \
		-DNAIVE=$@ -o $(OUTPUT_DIR)/$@ \
		$(SRCS_common)

sse_transpose: $(SRCS_common)
	$(CC) $(CFLAGS) -msse -mavx \
		-DSSE=$@ -o $(OUTPUT_DIR)/$@ \
		$(SRCS_common)

sse_prefetch_transpose: $(SRCS_common)
	$(CC) $(CFLAGS) -msse -mavx \
		-DSSE_PREFETCH=$@ -o $(OUTPUT_DIR)/$@ \
		$(SRCS_common)

avx_transpose: $(SRCS_common)
	$(CC) $(CFLAGS) -msse -mavx \
		-DAVX=$@ -o $(OUTPUT_DIR)/$@ \
		$(SRCS_common)

avx_prefetch_transpose: $(SRCS_common)
	$(CC) $(CFLAGS) -msse -mavx \
		-DAVX_PREFETCH=$@ -o $(OUTPUT_DIR)/$@ \
		$(SRCS_common)

check_execute:
	if [ ! -d executes ]; then mkdir executes; fi

verify: verify.c
	$(CC) $(CFLAGS) -msse -mavx \
		-DMAIN=$@ -o $(OUTPUT_DIR)/$@ \
		verify.c

.PHONY: clean
clean:
	rm -rf $(OUTPUT_DIR)
