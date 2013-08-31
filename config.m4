PHP_ARG_ENABLE(bitstore, whether to enable bit storage support,

[ --enable-bitstore Enable bit storage support])



if test "$PHP_BITSTORE" = "yes"; then

 AC_DEFINE(HAVE_HELLO, 1, [Whether you have bitstore])

 PHP_NEW_EXTENSION(bitstore, ../bitstore.c bitstore_php.c, $ext_shared)

fi