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
  | Author: Michael Maclean <mgdm@php.net>                               |
  +----------------------------------------------------------------------+
*/

/* $Id: header 252479 2008-02-07 19:39:50Z iliaa $ */

#ifndef PHP_RSVG_H
#define PHP_RSVG_H

#define PHP_RSVG_VERSION "0.1.0-alpha"

extern zend_module_entry rsvg_module_entry;
#define phpext_rsvg_ptr &rsvg_module_entry

extern zend_object_handlers rsvg_std_object_handlers;

zend_class_entry *rsvg_ce_rsvgexception;

#ifdef PHP_WIN32
#	define PHP_RSVG_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#	define PHP_RSVG_API __attribute__ ((visibility("default")))
#else
#	define PHP_RSVG_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

#include <glib.h>
#include <librsvg/rsvg.h>
#include <librsvg/rsvg-cairo.h>
#include "php_cairo_api.h"

typedef struct _rsvg_handle_object {
	zend_object std;
	RsvgHandle *handle;
} rsvg_handle_object;


PHP_MINIT_FUNCTION(rsvg);
PHP_MSHUTDOWN_FUNCTION(rsvg);
PHP_MINFO_FUNCTION(rsvg);

PHP_FUNCTION(rsvg_create);
PHP_FUNCTION(rsvg_render);

#ifdef ZTS
#define RSVG_G(v) TSRMG(rsvg_globals_id, zend_rsvg_globals *, v)
#else
#define RSVG_G(v) (rsvg_globals.v)
#endif

#endif	/* PHP_RSVG_H */

/* turn error handling to exception mode and restore */
/* Borrowed from pecl/cairo, ta auroraeosrose */
#if PHP_VERSION_ID >= 50300
/* 5.3 version of the macros */
#define PHP_RSVG_ERROR_HANDLING(force_exceptions) \
    zend_error_handling error_handling; \
    if(force_exceptions || getThis()) { \
        zend_replace_error_handling(EH_THROW, rsvg_ce_rsvgexception, &error_handling TSRMLS_CC); \
    }

#define PHP_RSVG_RESTORE_ERRORS(force_exceptions) \
    if(force_exceptions || getThis()) { \
        zend_restore_error_handling(&error_handling TSRMLS_CC); \
    }

#else
/* 5.2 versions of the macros */
#define PHP_RSVG_ERROR_HANDLING(force_exceptions) \
    if(force_exceptions || getThis()) { \
        php_set_error_handling(EH_THROW, rsvg_ce_rsvgexception TSRMLS_CC); \
    }

#define PHP_RSVG_RESTORE_ERRORS(force_exceptions) \
    if(force_exceptions || getThis()) { \
        php_std_error_handling(); \
    }

#endif


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
