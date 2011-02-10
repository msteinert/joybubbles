#serial 1

# JOY_TRY_CC_FLAGS(flags, [action-if-true], [action-if-false])
# -----------------------------------------------------------
# Test if one or more compiler flags are supported.
AC_DEFUN([JOY_TRY_CC_FLAGS],
[dnl
AC_REQUIRE([AC_PROG_CC])
for flag in "$1"
do
	jb_cflags=$CFLAGS
	CFLAGS="$flag $CFLAGS"
	AC_MSG_CHECKING([if compiler accepts '$flag'])
	AC_TRY_COMPILE([], [],
		[AC_MSG_RESULT([yes])
		CFLAGS=$jb_cflags
		$2],
		[AC_MSG_RESULT([no])
		CFLAGS=$jb_cflags
		$3])
done
])dnl

# JOY_TRY_LD_FLAGS(flags, [action-if-true], [action-if-false])
# -----------------------------------------------------------
# Test if one or more linker flags are supported.
AC_DEFUN([JOY_TRY_LD_FLAGS],
[dnl
for flag in "$1"
do
	jb_ldflags=$LDFLAGS
	LDFLAGS="$flag $LDFLAGS"
	AC_MSG_CHECKING([if linker accepts '$flag'])
	AC_TRY_LINK([], [],
		[AC_MSG_RESULT([yes])
		LDFLAGS=$jb_ldflags
		$2],
		[AC_MSG_RESULT([no])
		LDFLAGS=$jb_ldflags
		$3])
done
])dnl

# JOY_CFLAGS(flags)
# ----------------
# Enable compiler flags.
AC_DEFUN([JOY_CFLAGS],
[dnl
for flag in "$1"
do
	case " $CFLAGS " in #(
	*[[\ \	]]$flag[[\ \	]]*) :
		;; #(
	*) :
		CFLAGS="$CFLAGS $flag" ;; #(
	esac
done
])dnl

# JOY_LDFLAGS(flags)
# -----------------
# Enable linker flags.
AC_DEFUN([JOY_LDFLAGS],
[dnl
for flag in "$1"
do
	case " $LDFLAGS " in #(
	*[[\ \	]]$flag[[\ \	]]*) :
		;; #(
	*) :
		LDFLAGS="$LDFLAGS $flag" ;; #(
	esac
done
])dnl
