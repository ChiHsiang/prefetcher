CC ?= gcc
CFLAGS = -std=gnu99 -O0 -Wall -Wextra

EXEC = main naive_transpose sse_transpose sse_prefetch_transpose avx_transpose avx_prefetch_transpose

GIT_HOOKS := .git/hooks/applied

.PHONY: all
all: $(EXEC)

$(GIT_HOOKS):
	@scripts/install-git-hooks
	@echo

SRCS_common = main.c

main: $(SRCS_common)
	$(CC) $(CFLAGS) -msse -mavx \
		-DMAIN=$@ -o $@ \
		$(SRCS_common)

naive_transpose: $(SRCS_common)
	$(CC) $(CFLAGS) -mavx \
		-DNAIVE=$@ -o $@ \
		$(SRCS_common)

sse_transpose: $(SRCS_common)
	$(CC) $(CFLAGS) -msse -mavx \
		-DSSE=$@ -o $@ \
		$(SRCS_common)

sse_prefetch_transpose: $(SRCS_common)
	$(CC) $(CFLAGS) -msse -mavx \
		-DSSE_PREFETCH=$@ -o $@ \
		$(SRCS_common)

avx_transpose: $(SRCS_common)
	$(CC) $(CFLAGS) -msse -mavx \
		-DAVX=$@ -o $@ \
		$(SRCS_common)

avx_prefetch_transpose: $(SRCS_common)
	$(CC) $(CFLAGS) -msse -mavx \
		-DAVX_PREFETCH=$@ -o $@ \
		$(SRCS_common)

.PHONY: clean
clean:
	$(RM) $(EXEC)
