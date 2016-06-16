/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2010 The PHP Group                                |
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

/* $Id: header 297205 2010-03-30 21:09:07Z johannes $ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_safenet.h"

#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

/* CURL FUNCTION BEGIN*/
struct string {
	char *ptr;
	size_t len;
};

void init_string(struct string *s) {
	s->len = 0;
	s->ptr = malloc(s->len+1);
	if (s->ptr == NULL) {
		fprintf(stderr, "malloc() failed\n");
		exit(EXIT_FAILURE);
	}
	s->ptr[0] = '\0';
}

size_t writefunc(void *ptr, size_t size, size_t nmemb, struct string *s)
{
	size_t new_len = s->len + size*nmemb;
	s->ptr = realloc(s->ptr, new_len+1);
	if (s->ptr == NULL) {
		fprintf(stderr, "realloc() failed\n");
		exit(EXIT_FAILURE);
	}
	memcpy(s->ptr+s->len, ptr, size*nmemb);
	s->ptr[new_len] = '\0';
	s->len = new_len;

	return size*nmemb;
}

char * safenet(char *url, char *mode, char *key, char *in )
{ 
	CURL *curl;
	CURLcode res;
	char *fields;
	char *data;
	//int timeout = 10;
	int timeout = INI_INT("safenet.timeout");
	//curl_global_init(CURL_GLOBAL_ALL);
	/* get a curl handle */ 
	curl = curl_easy_init();
	if(curl) {
		struct string s;
		init_string(&s); 

		asprintf(&fields, "mode=%s&keyname=%s&input=%s", mode, key, in);    

		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURLOPT_USERAGENT, "safenet/1.0 by netkiller <netkiller@msn.com>");
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &s);
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, fields);
		curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeout);
 
		/* Perform the request, res will get the return code */ 
		res = curl_easy_perform(curl);
		/* Check for errors */ 
		if(res != CURLE_OK){
			//fprintf(stderr, "curl_easy_perform() failed: %s\n",
			strcpy(data, in);
		}else{
			asprintf(&data, "%s", s.ptr);
		}
		curl_easy_strerror(res);

		//printf("Encrypt: %s\n", data);

		free(s.ptr);
		/* always cleanup */ 
		curl_easy_cleanup(curl);
	}

	//printf("POST: %s\r\n", fields);
	return data;
	//curl_global_cleanup();
}
/* CURL FUNCTION END*/


/* If you declare any globals in php_safenet.h uncomment this:
ZEND_DECLARE_MODULE_GLOBALS(safenet)
*/

/* True global resources - no need for thread safety here */
static int le_safenet;

/* {{{ safenet_functions[]
 *
 * Every user visible function must have an entry in safenet_functions[].
 */
zend_function_entry safenet_functions[] = {
	PHP_FE(confirm_safenet_compiled,	NULL)		/* For testing, remove later. */
	PHP_FE(safenet_encrypt, NULL)
	PHP_FE(safenet_decrypt, NULL)
	{NULL, NULL, NULL}	/* Must be the last line in safenet_functions[] */
};
/* }}} */

/* {{{ safenet_module_entry
 */
zend_module_entry safenet_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	"safenet",
	safenet_functions,
	PHP_MINIT(safenet),
	PHP_MSHUTDOWN(safenet),
	PHP_RINIT(safenet),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(safenet),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(safenet),
#if ZEND_MODULE_API_NO >= 20010901
	"0.1", /* Replace with version number for your extension */
#endif
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_SAFENET
ZEND_GET_MODULE(safenet)
#endif

/* {{{ PHP_INI
 */

PHP_INI_BEGIN()
    //STD_PHP_INI_ENTRY("safenet.global_value",      "42", PHP_INI_ALL, OnUpdateLong, global_value, zend_safenet_globals, safenet_globals)
    //STD_PHP_INI_ENTRY("safenet.global_string", "foobar", PHP_INI_ALL, OnUpdateString, global_string, zend_safenet_globals, safenet_globals)
    PHP_INI_ENTRY("safenet.url", "http://localhost/", PHP_INI_ALL, NULL)
    PHP_INI_ENTRY("safenet.key", "key01", PHP_INI_ALL, NULL)
    PHP_INI_ENTRY("safenet.timeout", "20", PHP_INI_ALL, NULL)
PHP_INI_END()

/* }}} */

/* {{{ php_safenet_init_globals
 */
/* Uncomment this function if you have INI entries
static void php_safenet_init_globals(zend_safenet_globals *safenet_globals)
{
	safenet_globals->global_value = 0;
	safenet_globals->global_string = NULL;
}
*/
/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(safenet)
{
	
	REGISTER_INI_ENTRIES();
	
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(safenet)
{
	
	UNREGISTER_INI_ENTRIES();
	
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(safenet)
{
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(safenet)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(safenet)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "safenet support", "enabled");
	php_info_print_table_row(2, "author", "Neo Chan<netkiller@msn.com>");
	php_info_print_table_row(2, "safenet.url", INI_STR("safenet.url"));
	php_info_print_table_row(2, "safenet.key", INI_STR("safenet.key"));
	php_info_print_table_row(2, "safenet.timeout", INI_STR("safenet.timeout"));
	php_info_print_table_end();

	/* Remove comments if you have entries in php.ini
	DISPLAY_INI_ENTRIES();
	*/
}
/* }}} */


/* Remove the following function when you have succesfully modified config.m4
   so that your module can be compiled into PHP, it exists only for testing
   purposes. */

/* Every user-visible function in PHP should document itself in the source */
/* {{{ proto string confirm_safenet_compiled(string arg)
   Return a string to confirm that the module is compiled in */
PHP_FUNCTION(confirm_safenet_compiled)
{
	char *arg = NULL;
	int arg_len, len;
	char *strg;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &arg, &arg_len) == FAILURE) {
		return;
	}

	len = spprintf(&strg, 0, "Congratulations! You have successfully modified ext/%.78s/config.m4. Module %.78s is now compiled into PHP.", "safenet", arg);
	RETURN_STRINGL(strg, len, 0);
}
/* }}} */
/* The previous line is meant for vim and emacs, so it can correctly fold and 
   unfold functions in source code. See the corresponding marks just before 
   function definition, where the functions purpose is also documented. Please 
   follow this convention for the convenience of others editing your code.
*/

PHP_FUNCTION(safenet_encrypt)
{
	char *arg = NULL;
	int arg_len, len;
	char *strg;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &arg, &arg_len) == FAILURE) {
		return;
	}

	len = spprintf(&strg, 0, "%s", safenet(INI_STR("safenet.url"), "encrypt", INI_STR("safenet.key"), arg));
	RETURN_STRINGL(strg, len, 0);
}

PHP_FUNCTION(safenet_decrypt)
{
	char *arg = NULL;
	int arg_len, len;
	char *strg;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &arg, &arg_len) == FAILURE) {
		return;
	}
    
    if(arg_len!=512)
    {
		len = spprintf(&strg, 0, "%s", arg);
    }
    else
    {
	len = spprintf(&strg, 0, "%s", safenet(INI_STR("safenet.url"), "decrypt", INI_STR("safenet.key"), arg));
	}
	RETURN_STRINGL(strg, len, 0);
}


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
