TEMPLATE = lib
CONFIG -= qt
CONFIG += generateC

DISTFILES +=  $(HOME)/tool-inst/share/taste-types/taste-types.asn
DISTFILES += can_component_linux.msc
DISTFILES += interfaceview.xml
DISTFILES += work/binaries/*.msc
DISTFILES += work/binaries/coverage/index.html
DISTFILES += work/binaries/filters
DISTFILES += work/system.asn

DISTFILES += deploymentview.dv.xml
DISTFILES += can_component_linux.asn
DISTFILES += can_component_linux.acn
include(work/taste.pro)
message($$DISTFILES)

