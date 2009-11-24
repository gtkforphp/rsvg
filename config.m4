dnl
dnl $ Id: rsvg 1.0.1$
dnl

PHP_ARG_WITH(rsvg, for Cairo graphics library support,
[  --with-rsvg            Enable Cairo support], yes)

if test "$PHP_RSVG" != "no"; then
  export OLD_CPPFLAGS="$CPPFLAGS"
  export CPPFLAGS="$CPPFLAGS $INCLUDES -DHAVE_RSVG"

  AC_MSG_CHECKING(PHP version)
  AC_TRY_COMPILE([#include <php_version.h>], [
#if PHP_VERSION_ID < 50200
#error  this extension requires at least PHP version 5.2.0
#endif
],
[AC_MSG_RESULT(ok)],
[AC_MSG_ERROR([need at least PHP 5.2.0])])

  export CPPFLAGS="$OLD_CPPFLAGS"

  PHP_SUBST(RSVG_SHARED_LIBADD)
  AC_DEFINE(HAVE_RSVG, 1, [ ])

  PHP_NEW_EXTENSION(rsvg, rsvg.c, $ext_shared)

  EXT_RSVG_HEADERS="php_rsvg_api.h"

  ifdef([PHP_INSTALL_HEADERS], [
    PHP_INSTALL_HEADERS(ext/rsvg, $EXT_RSVG_HEADERS)
  ])

  if test "$PHP_RSVG" != "no"; then
      RSVG_CHECK_DIR=$PHP_RSVG
      RSVG_TEST_FILE=/include/rsvg.h
      RSVG_LIBNAME=rsvg
  fi
  condition="$RSVG_CHECK_DIR$RSVG_TEST_FILE"

  if test -r $condition; then
   RSVG_DIR=$RSVG_CHECK_DIR
     CFLAGS="$CFLAGS -I$RSVG_DIR/include"
   LDFLAGS=`$RSVG_DIR/bin/rsvg-config --libs`
  else
    AC_MSG_CHECKING(for pkg-config)
  
    if test ! -f "$PKG_CONFIG"; then
      PKG_CONFIG=`which pkg-config`
    fi

      if test -f "$PKG_CONFIG"; then
        AC_MSG_RESULT(found)
        AC_MSG_CHECKING(for rsvg)
    
        if $PKG_CONFIG --exists librsvg-2.0; then
            if $PKG_CONFIG --atleast-version=1.14 librsvg-2.0; then
                rsvg_version_full=`$PKG_CONFIG --modversion librsvg-2.0`
                AC_MSG_RESULT([found $rsvg_version_full])
                RSVG_LIBS="$LDFLAGS `$PKG_CONFIG --libs librsvg-2.0`"
                CAIRO_LIBS="$LDFLAGS `$PKG_CONFIG --libs cairo`"
                RSVG_INCS="$CFLAGS `$PKG_CONFIG --cflags-only-I librsvg-2.0`"
                CAIRO_INCS="$CFLAGS `$PKG_CONFIG --cflags-only-I cairo`"
                PHP_EVAL_INCLINE($RSVG_INCS)
                PHP_EVAL_INCLINE($CAIRO_INCS)
                PHP_EVAL_LIBLINE($RSVG_LIBS, RSVG_SHARED_LIBADD)
                PHP_EVAL_LIBLINE($CAIRO_LIBS, RSVG_SHARED_LIBADD)
                AC_DEFINE(HAVE_RSVG, 1, [whether rsvg exists in the system])
            else
                AC_MSG_RESULT(too old)
                AC_MSG_ERROR(Ooops ! You need at least rsvg 1.14)
            fi
        else
            AC_MSG_RESULT(not found)
            AC_MSG_ERROR(Ooops ! no rsvg detected in the system)
        fi
      else
        AC_MSG_RESULT(not found)
        AC_MSG_ERROR(Ooops ! no pkg-config found .... )
      fi
   fi

   AC_MSG_CHECKING(for cairo php extension)
   if test -f "$phpincludedir/ext/cairo/php_cairo_api.h"; then
       PHP_ADD_INCLUDE($phpincludedir/ext/cairo)
       PHP_DEF_HAVE(CAIRO)
       AC_MSG_RESULT(yes)
   else
       AC_MSG_RESULT(no)
       AC_MSG_ERROR(cairo php extension not found.)
   fi  
fi
