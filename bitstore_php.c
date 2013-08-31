#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <zend_compile.h>

#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/types.h>
#include "php.h"
#include "bitstore_php.h"
#include <bitstore.h>

static function_entry bitstore_functions[] = {
  PHP_FE(bs_alloc, NULL)
  PHP_FE(bs_free, NULL)
  PHP_FE(bs_get, NULL)
  PHP_FE(bs_switch, NULL)
  PHP_FE(bs_max, NULL)
  PHP_FE(bs_mmap, NULL)
  PHP_FE(bs_unmap, NULL)
  {NULL, NULL, NULL}
};

zend_module_entry bitstore_module_entry = {

#if ZEND_MODULE_API_NO >= 20010901
  STANDARD_MODULE_HEADER,
#endif
  PHP_BITSTORE_EXTNAME,
  bitstore_functions,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,

#if ZEND_MODULE_API_NO >= 20010901
  PHP_BITSTORE_VERSION,
#endif
  STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_BITSTORE
ZEND_GET_MODULE(bitstore)
#endif

int bs_type;

static void rsclean(zend_rsrc_list_entry *rsrc TSRMLS_DC)
{
    struct bitstore *storage = (struct bitstore *)rsrc->ptr;
	if(storage) {
		if(storage->addr) {
			free(storage->addr);
		}

		free(storage);
	}
}

PHP_MINIT_FUNCTION(bitstore)
{
	bs_type = zend_register_list_destructors_ex(rsclean, NULL, "bitstore", module_number);
	return SUCCESS;
}

PHP_FUNCTION(bs_alloc)
{
    int mem_id;
	size_t mem_size;
    struct bitstore *storage;

	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &mem_size) == FAILURE) {
		RETURN_NULL();
	}
	
	storage = bstore_alloc(mem_size);
	mem_id = zend_list_insert(storage, bs_type);
	
	RETURN_LONG(mem_id);
}

PHP_FUNCTION(bs_mmap)
{
	int mem_id, len;
	char *sfile;
	struct bitstore *storage;

	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &sfile, &len) == FAILURE) {
		RETURN_NULL();
	}
	
	storage = bstore_mmap(sfile);
	mem_id = zend_list_insert(storage, bs_type);
	
	RETURN_LONG(mem_id);
}

PHP_FUNCTION(bs_unmap)
{
	long sid;
	int type;
	struct bitstore *storage;

	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &sid) == FAILURE) {
		RETURN_NULL();
	}

	storage = zend_list_find(sid, &type);
	bstore_unmap(storage);
	zend_list_delete(storage); // @todo Test it. Warning, int expected
	
	RETURN_BOOL(1);
}

PHP_FUNCTION(bs_free)
{
	long sid;
	int type;
	struct bitstore *storage;
	
	
	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &sid) == FAILURE) {
		RETURN_NULL();
	}

	storage = zend_list_find(sid, &type);
	bstore_free(storage);
	zend_list_delete(storage); // @todo Test it. Warning, int expected
	
	RETURN_BOOL(1);
}

PHP_FUNCTION(bs_get)
{
	int sid, len, type, bit;
	char *hash;
	struct bitstore *storage;

	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ls", &sid, &hash, &len) == FAILURE) {
		RETURN_NULL();
	}

	storage = zend_list_find(sid, &type);
	bit = bstore_get(storage, hash);

	RETURN_BOOL(bit);
}

PHP_FUNCTION(bs_switch)
{
	int sid, type, len;
	struct bitstore *storage;
	char *hash;

	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ls", &sid, &hash, &len) == FAILURE) {
		RETURN_NULL();
	}

	storage = zend_list_find(sid, &type);
	bstore_switch(storage, hash);

	RETURN_BOOL(1);
}

PHP_FUNCTION(bs_max)
{
	int sid, type;
	char max[18];
	struct bitstore *storage;
	
	if(zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &sid) == FAILURE) {
		RETURN_NULL();
	}
	
	storage = zend_list_find(sid, &type);
	bstore_max(storage, max);

	RETURN_STRING(max, 1);
}