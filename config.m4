dnl $Id$
dnl config.m4 for extension safenet

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

dnl PHP_ARG_WITH(safenet, for safenet support,
dnl Make sure that the comment is aligned:
dnl [  --with-safenet             Include safenet support])

dnl Otherwise use enable:

PHP_ARG_ENABLE(safenet, whether to enable safenet support,
Make sure that the comment is aligned:
[  --enable-safenet           Enable safenet support])

if test "$PHP_SAFENET" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-safenet -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/safenet.h"  # you most likely want to change this
  dnl if test -r $PHP_SAFENET/$SEARCH_FOR; then # path given as parameter
  dnl   SAFENET_DIR=$PHP_SAFENET
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for safenet files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       SAFENET_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$SAFENET_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the safenet distribution])
  dnl fi

  dnl # --with-safenet -> add include path
  dnl PHP_ADD_INCLUDE($SAFENET_DIR/include)

  dnl # --with-safenet -> check for lib and symbol presence
  dnl LIBNAME=safenet # you may want to change this
  dnl LIBSYMBOL=safenet # you most likely want to change this 

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $SAFENET_DIR/lib, SAFENET_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_SAFENETLIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong safenet lib version or lib not found])
  dnl ],[
  dnl   -L$SAFENET_DIR/lib -lm -ldl
  dnl ])
  dnl
  dnl PHP_SUBST(SAFENET_SHARED_LIBADD)

  PHP_NEW_EXTENSION(safenet, safenet.c, $ext_shared)
fi
