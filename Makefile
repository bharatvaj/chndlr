# chndlr - simple xdg-open replacement with fallback

include config.mk

SRC = chndlr.c
OBJ = ${SRC:.c=.o}

all: options chndlr

options:
	@echo chndlr build options:
	@echo "CFLAGS   = ${CFLAGS}"
	@echo "LDFLAGS  = ${LDFLAGS}"
	@echo "CC       = ${CC}"

.c.o:
	${CC} -c ${CFLAGS} $<

${OBJ}: config.mk config.h

chndlr: ${OBJ}
	${CC} -o $@ ${OBJ} ${LDFLAGS}

clean:
	@echo cleaning
	@rm -f chndlr ${OBJ}

install: all
	@cp -f chndlr /usr/bin/xdg-open
	@chmod 755 /usr/bin/xdg-open

uninstall:
	-rm /usr/bin/xdg-open
