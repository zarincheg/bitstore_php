/* 
 * File:   bitstore_php.h
 * Author: zarin
 *
 * Created on November 25, 2011, 2:12 AM
 */

#ifndef PHP_BITSTORE_H

    #define PHP_BITSTORE_H 1
    #define PHP_BITSTORE_VERSION "0.2.1"
    #define PHP_BITSTORE_EXTNAME "bitstore"

	PHP_MINIT_FUNCTION(bitstore);
	
	PHP_FUNCTION(bs_alloc);
	PHP_FUNCTION(bs_free);
	PHP_FUNCTION(bs_get);
	PHP_FUNCTION(bs_switch);
	PHP_FUNCTION(bs_max);
	PHP_FUNCTION(bs_mmap);
	PHP_FUNCTION(bs_unmap);

    extern zend_module_entry bitstore_module_entry;

    #define phpext_bitstore_ptr &bitstore_module_entry
#endif