# Object lists built from the target-specific source lists:
# 
# LOCSSRC: Assembly source code files in this directory.
# CPUSSRC: Assembly source code files in the common/cpu directory.
# LOCCSRC: C source code files in this directory.
# COMCSRC: C source code files in the common/monitor directory.
# CPUCSRC: C source code files in the common/cpu directory.
#
LOCSOBJ		= $(LOCSSRC:%.S=%.o)
LOCCOBJ		= $(LOCCSRC:%.c=%.o)
CPUSOBJ		= $(CPUSSRC:%.S=%.o)
CPUCOBJ		= $(CPUCSRC:%.c=%.o)
COMCOBJ		= $(COMCSRC:%.c=%.o)

# JZLIBOBJ:  C source code files used by jffs2's version of zlib.
JZLIBOBJ	= $(JZLIBSRC:%.c=%.o)

# ZLIBOBJ:  C source code files in the common/zlib directory.
ZLIBOBJ		= $(ZLIBSRC:%.c=%.o)

# GLIBOBJ:  C source code files in the common/glib directory.
GLIBOBJ		= $(GLIBSRC:%.c=%.o)

FLASHOBJ	= $(FLASHSRC:%.c=%.o)
IODEVOBJ	= $(IODEVSRC:%.c=%.o)
