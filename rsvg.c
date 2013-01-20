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
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_rsvg.h"

zend_object_handlers rsvg_std_object_handlers;

zend_class_entry *rsvg_ce_rsvg;

/* {{{ proto rsvg_create(string data)
       Create a new Rsvg object from SVG data */
PHP_FUNCTION(rsvg_create)
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

	object_init_ex(return_value, rsvg_ce_rsvg);
	handle_object = (rsvg_handle_object *)rsvg_handle_object_get(return_value TSRMLS_CC);
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

/* {{{ proto Rsvg::__construct(string data)
       Create a new Rsvg object from SVG data */
PHP_METHOD(Rsvg, __construct)
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

/* {{{ proto Rsvg::createFromFile(string|resource file)
       Create a new Rsvg object from SVG data from a file */
PHP_METHOD(Rsvg, createFromFile)
{
	rsvg_handle_object *handle_object = NULL;
	zval *stream_zval = NULL;
	GError *error = NULL;
	php_stream *stream;
	zend_bool owned_stream = 0;
	char *data = NULL;
	long data_len;
	
	PHP_RSVG_ERROR_HANDLING(TRUE)
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &stream_zval) == FAILURE) {
		PHP_RSVG_RESTORE_ERRORS(TRUE)
		return;
	}
	PHP_RSVG_RESTORE_ERRORS(TRUE)
	
	object_init_ex(return_value, rsvg_ce_rsvg);
	handle_object = (rsvg_handle_object *)zend_object_store_get_object(return_value TSRMLS_CC);

	if(Z_TYPE_P(stream_zval) == IS_STRING) {
		stream = php_stream_open_wrapper(Z_STRVAL_P(stream_zval), "rb", REPORT_ERRORS|ENFORCE_SAFE_MODE, NULL);
		owned_stream = 1;
	} else if(Z_TYPE_P(stream_zval) == IS_RESOURCE) {
		php_stream_from_zval(stream, &stream_zval);
	} else {
		zend_throw_exception(rsvg_ce_rsvgexception, "Rsvg::createFromFile expects parameter 1 to be a string or a stream resource", 0 TSRMLS_CC);
		return;
	}

	data_len = php_stream_copy_to_mem(stream, &data, PHP_STREAM_COPY_ALL, 0);

	rsvg_init();
	handle_object->handle = rsvg_handle_new_from_data(data, data_len, &error);
	efree(data);

	if(owned_stream) {
		php_stream_close(stream);
	}
	
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

/* {{{ proto rsvg_create_from_file(string|resource file)
       Create a new Rsvg object from SVG data from a file */
PHP_FUNCTION(rsvg_create_from_file)
{
	rsvg_handle_object *handle_object = NULL;
	zval *stream_zval = NULL;
	GError *error = NULL;
	php_stream *stream;
	zend_bool owned_stream = 0;
	char *data = NULL;
	long data_len;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &stream_zval) == FAILURE) {
		return;
	}
	
	object_init_ex(return_value, rsvg_ce_rsvg);
	handle_object = (rsvg_handle_object *)zend_object_store_get_object(return_value TSRMLS_CC);

	if(Z_TYPE_P(stream_zval) == IS_STRING) {
		stream = php_stream_open_wrapper(Z_STRVAL_P(stream_zval), "rb", REPORT_ERRORS|ENFORCE_SAFE_MODE, NULL);
		owned_stream = 1;
	} else if(Z_TYPE_P(stream_zval) == IS_RESOURCE) {
		php_stream_from_zval(stream, &stream_zval);
	} else {
		zend_error(E_WARNING, "Rsvg::createFromFile expects parameter 1 to be a string or a stream resource");
		RETURN_NULL();
	}

	data_len = php_stream_copy_to_mem(stream, &data, PHP_STREAM_COPY_ALL, 0);

	rsvg_init();
	handle_object->handle = rsvg_handle_new_from_data(data, data_len, &error);
	efree(data);

	if(error != NULL) {
		zend_error(E_WARNING, "%s", error->message);
		g_error_free(error);
		RETURN_NULL();
	}

	if(handle_object->handle == NULL) {
		zend_throw_exception(rsvg_ce_rsvgexception, "Could not create the RSVG object", 0 TSRMLS_CC);
		RETURN_NULL();
	}
}
/* }}} */

/* {{{ proto array Rsvg::getDimensions([string element])
       proto array rsvg_get_dimensions([string element])
	   Get the dimensions of an SVG. Pass an element ID to get that element's size. */
PHP_FUNCTION(rsvg_get_dimensions)
{
	zval *rsvg_zval;
	rsvg_handle_object *handle_object;
	RsvgDimensionData dimension_data;
	char *id = NULL;
	long id_len;
	
	PHP_RSVG_ERROR_HANDLING(FALSE)
	if(zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "O|s", &rsvg_zval, rsvg_ce_rsvg, &id, &id_len) == FAILURE) {
		PHP_RSVG_RESTORE_ERRORS(FALSE)
		return;
	}
	PHP_RSVG_RESTORE_ERRORS(FALSE)

	handle_object = rsvg_handle_object_get(rsvg_zval TSRMLS_CC);

	if(id == NULL) {
		rsvg_handle_get_dimensions(handle_object->handle, &dimension_data);
	} else {
		rsvg_handle_get_dimensions_sub(handle_object->handle, &dimension_data, (const char *)id);
	}

	array_init(return_value);
	add_assoc_long(return_value, "width", dimension_data.width);
	add_assoc_long(return_value, "height", dimension_data.height);
	add_assoc_double(return_value, "em", dimension_data.em);
	add_assoc_double(return_value, "ex", dimension_data.ex);
}

/* }}} */

/* {{{ proto array Rsvg::getTitle([string element])
       proto array rsvg_get_title([string element])
	   Get the SVG's title */
PHP_FUNCTION(rsvg_get_title)
{
	zval *rsvg_zval;
	rsvg_handle_object *handle_object;
	char *title = NULL;
	
	PHP_RSVG_ERROR_HANDLING(FALSE)
	if(zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "O", &rsvg_zval, rsvg_ce_rsvg) == FAILURE) {
		PHP_RSVG_RESTORE_ERRORS(FALSE)
		return;
	}
	PHP_RSVG_RESTORE_ERRORS(FALSE)

	handle_object = rsvg_handle_object_get(rsvg_zval TSRMLS_CC);

	title = (char *)rsvg_handle_get_title(handle_object->handle);

	if(title != NULL) {
		RETURN_STRING(title, 1);
	} 
}

/* }}} */

/* {{{ proto array Rsvg::getDescription([string element])
       proto array rsvg_get_description([string element])
	   Get the SVG's description */
PHP_FUNCTION(rsvg_get_description)
{
	zval *rsvg_zval;
	rsvg_handle_object *handle_object;
	char *desc = NULL;
	
	PHP_RSVG_ERROR_HANDLING(FALSE)
	if(zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "O", &rsvg_zval, rsvg_ce_rsvg) == FAILURE) {
		PHP_RSVG_RESTORE_ERRORS(FALSE)
		return;
	}
	PHP_RSVG_RESTORE_ERRORS(FALSE)

	handle_object = rsvg_handle_object_get(rsvg_zval TSRMLS_CC);

	desc = (char *)rsvg_handle_get_desc(handle_object->handle);

	if(desc != NULL) {
		RETURN_STRING(desc, 1);
	} 
}

/* }}} */

/* {{{ proto array Rsvg::hasElement(string element)
	   proto array rsvg_has_element(string element)
	   Checks whether the element id exists in the SVG document. */
PHP_FUNCTION(rsvg_has_element)
{
	zval *rsvg_zval;
	rsvg_handle_object *handle_object;
	char *id = NULL;
	long id_len;
	
	PHP_RSVG_ERROR_HANDLING(FALSE)
	if(zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "Os", &rsvg_zval, rsvg_ce_rsvg, &id, &id_len) == FAILURE) {
		PHP_RSVG_RESTORE_ERRORS(FALSE)
		return;
	}
	PHP_RSVG_RESTORE_ERRORS(FALSE)

	handle_object = rsvg_handle_object_get(rsvg_zval TSRMLS_CC);
	RETURN_BOOL(rsvg_handle_has_sub(handle_object->handle, (const char*)id));	
}

/* }}} */

/* {{{ proto boolean Rsvg::render(CairoContext $cr, [string $id])
       proto boolean rsvg_render(CairoContext $cr, [string $id])
 	   Render the SVG data to a Cairo context. Passing an id of an 
	   element in the SVG will render only that element. */
PHP_FUNCTION(rsvg_render)
{
	zval *handle_zval, *context_zval;
	rsvg_handle_object *handle_object;
	cairo_context_object *context_object;
	zend_class_entry *cairo_ce_cairocontext;
	char *id = NULL;
	long id_len;
	int result;

	cairo_ce_cairocontext = php_cairo_get_context_ce();
	PHP_RSVG_ERROR_HANDLING(FALSE)
	if (zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "OO|s", &handle_zval, rsvg_ce_rsvg, &context_zval, cairo_ce_cairocontext, &id, &id_len) == FAILURE) {
		PHP_RSVG_RESTORE_ERRORS(FALSE)
		return;
	}
	PHP_RSVG_RESTORE_ERRORS(FALSE)

	handle_object = (rsvg_handle_object *)rsvg_handle_object_get(handle_zval TSRMLS_CC);
	context_object = (cairo_context_object *)cairo_context_object_get(context_zval TSRMLS_CC);

	if(id == NULL) {
		result = rsvg_handle_render_cairo(handle_object->handle, context_object->context);
	} else {
		result = rsvg_handle_render_cairo_sub(handle_object->handle, context_object->context, (const char*)id);
	}

	RETURN_BOOL(result);
}

/* }}} */

/* {{{ Object creation/destruction functions */
static void rsvg_object_destroy(void *object TSRMLS_DC)
{
    rsvg_handle_object *handle = (rsvg_handle_object *)object;
    zend_hash_destroy(handle->std.properties);
    FREE_HASHTABLE(handle->std.properties);

    if(handle->handle){
        g_object_unref(handle->handle);
    }    
    efree(object);
}

static zend_object_value rsvg_object_new(zend_class_entry *ce TSRMLS_DC)
{
    zend_object_value retval;
    rsvg_handle_object *handle;
    zval *temp;

    handle = ecalloc(1, sizeof(rsvg_handle_object));

    handle->std.ce = ce;
    handle->handle = NULL;

    ALLOC_HASHTABLE(handle->std.properties);
    zend_hash_init(handle->std.properties, 0, NULL, ZVAL_PTR_DTOR, 0);

#if PHP_VERSION_ID < 50399
	zend_hash_copy(handle->std.properties, &ce->default_properties, (copy_ctor_func_t) zval_add_ref,(void *) &temp, sizeof(zval *));
#else
	object_properties_init(&(handle->std), ce);
#endif

    retval.handle = zend_objects_store_put(handle, NULL, (zend_objects_free_object_storage_t)rsvg_object_destroy, NULL TSRMLS_CC);
    retval.handlers = &rsvg_std_object_handlers;
    return retval;
}
/* }}} */

/* {{{ rsvg_methods[] */
const zend_function_entry rsvg_methods[] = {
	PHP_ME(Rsvg, __construct, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(Rsvg, createFromFile, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME_MAPPING(getDimensions, rsvg_get_dimensions, NULL, ZEND_ACC_PUBLIC)
	PHP_ME_MAPPING(getTitle, rsvg_get_title, NULL, ZEND_ACC_PUBLIC)
	PHP_ME_MAPPING(getDescription, rsvg_get_description, NULL, ZEND_ACC_PUBLIC)
	PHP_ME_MAPPING(hasElement, rsvg_has_element, NULL, ZEND_ACC_PUBLIC)
	PHP_ME_MAPPING(render, rsvg_render, NULL, ZEND_ACC_PUBLIC)
	{NULL, NULL, NULL}	/* Must be the last line in rsvg_functions[] */
};
/* }}} */

/* {{{ rsvg_functions[] */
const zend_function_entry rsvg_functions[] = {
	PHP_FE(rsvg_create, NULL)
	PHP_FE(rsvg_create_from_file, NULL)
	PHP_FE(rsvg_get_dimensions, NULL)
	PHP_FE(rsvg_get_title, NULL)
	PHP_FE(rsvg_get_description, NULL)
	PHP_FE(rsvg_has_element, NULL)
	PHP_FE(rsvg_render, NULL)
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
	PHP_RSVG_VERSION,
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
    zend_class_entry ce;
    zend_class_entry direction_ce;
	zend_class_entry exception_ce;

    memcpy(&rsvg_std_object_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
    rsvg_std_object_handlers.clone_obj = NULL;

	INIT_CLASS_ENTRY(exception_ce, "RsvgException", NULL);
	rsvg_ce_rsvgexception = zend_register_internal_class_ex(&exception_ce, zend_exception_get_default(TSRMLS_C), "Exception" TSRMLS_CC);


    INIT_CLASS_ENTRY(ce, "Rsvg", rsvg_methods);
    rsvg_ce_rsvg = zend_register_internal_class(&ce TSRMLS_CC);
    rsvg_ce_rsvg->create_object = rsvg_object_new;

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
