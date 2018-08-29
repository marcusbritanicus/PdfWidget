TEMPLATE = subdirs
SUBDIRS += hexdump namedump cmapdump

hexdump.file = hexdump.pro
namedump.file = namedump.pro
cmapdump.file = cmapdump.pro

Resources.target = Resources
Resources.commands = cd ../source; sh generate.sh
POST_TARGETDEPS += Resources
QMAKE_EXTRA_TARGETS += Resources
