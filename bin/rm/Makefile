#	@(#)Makefile	8.1 (Berkeley) 5/31/93
# $FreeBSD$

.include <src.opts.mk>

PACKAGE=runtime
PROG=	rm
NO_SHARED?=	YES

LINKS=	${BINDIR}/rm ${BINDIR}/unlink
MLINKS=	rm.1 unlink.1

HAS_TESTS=
SUBDIR.${MK_TESTS}+= tests

.include <bsd.prog.mk>
