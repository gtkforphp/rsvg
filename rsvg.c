/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2008 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author:                                                              |
  +----------------------------------------------------------------------+
*/

/* $Id: header 252479 2008-02-07 19:39:50Z iliaa $ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_rsvg.h"

zend_object_handlers rsvg_std_object_handlers;

/* {{{ rsvg_functions[]
 *
 * Every user visible function must have an entry in rsvg_functions[].
 */
const zend_function_entry rsvg_functions[] = {
	{NULL, NULL, NULL}	/* Must be the last line in rsvg_functions[] */
};
/* }}} */

/* {{{ rsvg_module_entry
 */
zend_module_entry rsvg_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	"rsvg",
	rsvg_functions,
	PHP_MINIT(rsvg),
	PHP_MSHUTDOWN(rsvg),
	NULL,
	NULL,
	PHP_MINFO(rsvg),
#if ZEND_MODULE_API_NO >= 20010901
	"0.1", /* Replace with version number for your extension */
#endif
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_RSVG
ZEND_GET_MODULE(rsvg)
#endif

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(rsvg)
{
    memcpy(&rsvg_std_object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
    rsvg_std_object_handlers.clone_obj = NULL;

	zend_class_entry exception_ce;
	INIT_CLASS_ENTRY(exception_ce, "RsvgException", NULL);
	rsvg_ce_rsvgexception = zend_register_internal_class_ex(&exception_ce, zend_exception_get_default(TSRMLS_C), "Exception" TSRMLS_CC);

	PHP_MINIT(rsvg_handle)(INIT_FUNC_ARGS_PASSTHRU);

	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(rsvg)
{
	return SUCCESS;
}
/* }}} */

PHP_MINFO_FUNCTION(rsvg)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "rsvg support", "enabled");
	php_info_print_table_end();
}
/* }}} */


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
