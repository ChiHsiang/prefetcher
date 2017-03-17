CC ?= gcc
CFLAGS = -std=gnu99 -O0 -Wall -Wextra

EXEC = naive_transpose sse_transpose sse_prefetch_transpose

GIT_HOOKS := .git/hooks/applied

.PHONY: all
all: $(EXEC)

$(GIT_HOOKS):
	@scripts/install-git-hooks
	@echo

SRCS_common = main.c

naive_transpose: $(SRCS_common)
	$(CC) $(CFLAGS) -o $@ \
		$(SRCS_common)

sse_transpose: $(SRCS_common)
	$(CC) $(CFLAGS) -o $@ \
		$(SRCS_common)

sse_prefetch_transpose: $(SRCS_common)
	$(CC) $(CFLAGS) -o $@ \
		$(SRCS_common)

.PHONY: clean
clean:
	$(RM) $(EXEC)
