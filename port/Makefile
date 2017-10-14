# Created by: Hondareyte <lhondareyte@laposte.net>
# $FreeBSD$

PORTNAME=	noizebox
#PORTVERSION!=	git describe --tags
PORTVERSION=	1.5
CATEGORIES=	audio

MAINTAINER=	lhondareyte@laposte.net
WWW=		http://lhondareyte.free.fr/Noizebox
COMMENT=	ncurses SF2 synthesizer based on Fluidsynth

LEGAL_PACKAGE=	yes
LICENSE=	BSD2CLAUSE
NO_LICENSES_INSTALL=yes

#RUN_DEPENDS=	sysutils/spectro-450

PREFIX=		/Applications
USES=		pkgconfig

clean:
	@rm -rf work

extract:
	@${MKDIR} ${STAGEDIR}
	
build:
	
do-install:
	@${MKDIR} ${STAGEDIR}${PREFIX}
	@${COPYTREE_BIN} ../Noizebox  ${STAGEDIR}${PREFIX}/${PREFIX}
	@chmod 644  ${STAGEDIR}${PREFIX}/Noizebox/Resources/soundfont.conf

.include <bsd.port.mk>
