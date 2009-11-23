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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_rsvg.h"

zend_class_entry *rsvg_ce_rsvghandle;

PHP_RSVG_API zend_class_entry* php_rsvg_get_handle_ce() {
    return rsvg_ce_rsvghandle;
}

/* {{{ proto RsvgHandle::__construct(string data)
       Create a new RsvgHandle object from SVG data */
PHP_METHOD(RsvgHandle, __construct)
{
	rsvg_handle_object *handle_object = NULL;
	zval *data_zval = NULL;
	const char *data;
	long data_len;
	GError *error = NULL;
	
	PHP_RSVG_ERROR_HANDLING(TRUE)
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &data, &data_len) == FAILURE) {
		PHP_RSVG_RESTORE_ERRORS(TRUE)
		return;
	}

	PHP_RSVG_RESTORE_ERRORS(TRUE)
	if(data_len == 0) {
		zend_throw_exception(rsvg_ce_rsvgexception, "No SVG data supplied", 0 TSRMLS_CC);
		return;
	}

	handle_object = (rsvg_handle_object *)zend_object_store_get_object(getThis() TSRMLS_CC);
	rsvg_init();
	handle_object->handle = rsvg_handle_new_from_data(data, data_len, &error);

	if(error != NULL) {
		zend_throw_exception(rsvg_ce_rsvgexception, error->message, error->code TSRMLS_CC);
		g_error_free(error);
		return;
	}

	if(handle_object->handle == NULL) {
		zend_throw_exception(rsvg_ce_rsvgexception, "Could not create the RSVG object", 0 TSRMLS_CC);
		return;
	}

}

/* }}} */

/* {{{ proto boolean RsvgHandle::renderCairo(CairoContext $cr)
       proto boolean rsvg_handle_render_cairo(CairoContext $cr)
 	   Render the SVG data to a Cairo context */
PHP_FUNCTION(rsvg_handle_render_cairo)
{
	zval *handle_zval, *context_zval;
	rsvg_handle_object *handle_object;
	cairo_context_object *context_object;
	zend_class_entry *cairo_ce_cairocontext;
	int result;

	cairo_ce_cairocontext = php_cairo_get_context_ce();
	PHP_RSVG_ERROR_HANDLING(FALSE)
	if (zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "OO", &handle_zval, rsvg_ce_rsvghandle, &context_zval, cairo_ce_cairocontext) == FAILURE) {
		PHP_RSVG_RESTORE_ERRORS(FALSE)
		return;
	}
	PHP_RSVG_RESTORE_ERRORS(FALSE)

	handle_object = (rsvg_handle_object *)zend_object_store_get_object(handle_zval TSRMLS_CC);
	context_object = (cairo_context_object *)zend_object_store_get_object(context_zval TSRMLS_CC);

	result = rsvg_handle_render_cairo(handle_object->handle, context_object->context);
	RETURN_BOOL(result);
}


/* {{{ Object creation/destruction functions */
static void rsvg_handle_object_destroy(void *object TSRMLS_DC)
{
    rsvg_handle_object *handle = (rsvg_handle_object *)object;
    zend_hash_destroy(handle->std.properties);
    FREE_HASHTABLE(handle->std.properties);

    if(handle->handle){
        g_object_unref(handle->handle);
    }    
    efree(object);
}

static zend_object_value rsvg_handle_object_new(zend_class_entry *ce TSRMLS_DC)
{
    zend_object_value retval;
    rsvg_handle_object *handle;
    zval *temp;

    handle = ecalloc(1, sizeof(rsvg_handle_object));

    handle->std.ce = ce;
    handle->handle = NULL;

    ALLOC_HASHTABLE(handle->std.properties);
    zend_hash_init(handle->std.properties, 0, NULL, ZVAL_PTR_DTOR, 0);
    zend_hash_copy(handle->std.properties, &ce->default_properties, (copy_ctor_func_t) zval_add_ref,(void *) &temp, sizeof(zval *));
    retval.handle = zend_objects_store_put(handle, NULL, (zend_objects_free_object_storage_t)rsvg_handle_object_destroy, NULL TSRMLS_CC);
    retval.handlers = &rsvg_std_object_handlers;
    return retval;
}
/* }}} */

/* {{{ rsvg_handle_methods */
const zend_function_entry rsvg_handle_methods[] = {
	PHP_ME(RsvgHandle, __construct, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME_MAPPING(renderCairo, rsvg_handle_render_cairo, NULL, ZEND_ACC_PUBLIC)
    { NULL, NULL, NULL }
};
/* }}} */

/* {{{ PHP_MINIT_FUNCTION */
PHP_MINIT_FUNCTION(rsvg_handle)
{
    zend_class_entry handle_ce;
    zend_class_entry direction_ce;

    INIT_CLASS_ENTRY(handle_ce, "RsvgHandle", rsvg_handle_methods);
    rsvg_ce_rsvghandle = zend_register_internal_class(&handle_ce TSRMLS_CC);
    rsvg_ce_rsvghandle->create_object = rsvg_handle_object_new;

    return SUCCESS;
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
