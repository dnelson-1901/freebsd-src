.include <src.opts.mk>

PACKAGE=	runtime
PROG=	sed
SRCS=	compile.c main.c misc.c process.c
NO_SHARED?=	YES

HAS_TESTS=
SUBDIR.${MK_TESTS}+= tests

.include <bsd.prog.mk>
