.include <src.opts.mk>

PROG=	head
NO_SHARED?=	YES
LIBADD=	util

HAS_TESTS=
SUBDIR.${MK_TESTS}+= tests

.if ${MK_CASPER} != "no" && !defined(RESCUE)
LIBADD+=        casper
LIBADD+=        cap_fileargs
CFLAGS+=-DWITH_CASPER
.endif

.include <bsd.prog.mk>
