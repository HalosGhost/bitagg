PRJNM = bitagg
LIBNM = lib$(PRJNM)

SO_VER = 0.1

RTDIR = $(shell git rev-parse --show-toplevel)

SRCDIR = src
TSTDIR = tst
INCDIR = inc
BLDDIR = bld
OBJDIR = $(BLDDIR)/obj
DEPDIR = dep

CONFIGURED_DEPS = 

CPPFLAGS = -I$(INCDIR)
CFLAGS = -Og -ggdb3 -Wall -Wextra -Wpedantic -std=gnu18
LINKFLAGS = -lm -lxxhash

ifneq ($(CONFIGURED_DEPS),)
CPPFLAGS += $(shell pkg-config --cflags-only-I $(CONFIGURED_DEPS))
CFLAGS += $(shell pkg-config --cflags-only-other $(CONFIGURED_DEPS))
LINKFLAGS += $(shell pkg-config --libs $(CONFIGURED_DEPS))
endif

DATE = $(shell date +'%Y-%b-%d')

MKDIR = @mkdir -p --
RM = rm -rf --
LN = ln -sf --

SOURCES = $(patsubst $(SRCDIR)/main.c,,$(wildcard $(SRCDIR)/*))
OBJECTS = $(patsubst $(SRCDIR)%,$(OBJDIR)%,$(patsubst %.c,%.o,$(SOURCES)))
DEPENDS = $(patsubst $(SRCDIR)%,$(DEPDIR)%,$(patsubst %.c,%.d,$(SOURCES)))

TESTSRC = $(wildcard $(TSTDIR)/*.c)
TESTS   = $(patsubst $(TSTDIR)%,$(BLDDIR)/$(TSTDIR)%,$(patsubst %.c,%,$(TESTSRC)))

.PHONY: all clean check $(LIBNM) $(PRJNM)

all: $(LIBNM) check

check: $(TESTS)
	(for i in $^; do \
		printf '%s: [ PEND ]' $$i; \
		if ./$$i; then \
			printf '\r%s: [ PASS ]\n' $$i; \
		else \
			printf '\r%s: [ FAIL ]\n' $$i; \
		fi; \
	done)

clean:
	$(RM) $(BLDDIR) $(DEPDIR) tags

-include $(DEPENDS)

$(BLDDIR)/$(TSTDIR)/%: $(TSTDIR)/%.c $(BLDDIR)/$(LIBNM).a
	$(MKDIR) $(@D) $(DEPDIR)
	$(CC) $(CPPFLAGS) -fPIC $(CFLAGS) $^ -MMD -MP -MF $(DEPDIR)/$(@F).d -o $@ $(LINKFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(MKDIR) $(@D) $(DEPDIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -MMD -MP -MF $(DEPDIR)/$(@F:.o=.d) -o $@

$(PRJNM): $(BLDDIR)/$(PRJNM)

$(BLDDIR)/$(PRJNM): $(OBJECTS) $(OBJDIR)/main.o
	$(MKDIR) $(@D)
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ $^ $(LINKFLAGS) -L$(BLDDIR) -$(patsubst lib%,l%,$(LIBNM))

$(LIBNM): $(BLDDIR)/$(LIBNM).so $(BLDDIR)/$(LIBNM).a

$(BLDDIR)/$(LIBNM).so.$(SO_VER): $(OBJECTS)
	$(MKDIR) $(@D)
	$(CC) $(CPPFLAGS) -fPIE $(CFLAGS) -shared -o $@ $^ $(LINKFLAGS) -Wl,-soname,$(@F)

$(BLDDIR)/$(LIBNM).so: $(BLDDIR)/$(LIBNM).so.$(SO_VER)
	$(MKDIR) $(@D)
	$(LN) $(shell basename $<) $@

$(BLDDIR)/$(LIBNM).a: $(OBJECTS)
	$(MKDIR) $(@D)
	$(AR) rcs $@ $^

tags:
	find . -type f -iregex '.*\.[ch]\(xx\|pp\)?$$' | xargs ctags -a -f $@

$(V).SILENT:
