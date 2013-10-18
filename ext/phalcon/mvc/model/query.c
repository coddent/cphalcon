
#ifdef HAVE_CONFIG_H
#include "../../../ext_config.h"
#endif

#include <php.h>
#include "../../../php_ext.h"
#include "../../../ext.h"

#include <Zend/zend_operators.h>
#include <Zend/zend_exceptions.h>
#include <Zend/zend_interfaces.h>

#include "kernel/main.h"
#include "kernel/fcall.h"
#include "kernel/memory.h"
#include "kernel/exception.h"
#include "kernel/object.h"
#include "kernel/operators.h"
#include "kernel/array.h"
#include "kernel/concat.h"
#include "kernel/string.h"
#include "kernel/hash.h"
#include "ext/spl/spl_exceptions.h"


/*
 +------------------------------------------------------------------------+
 | Phalcon Framework                                                      |
 +------------------------------------------------------------------------+
 | Copyright (c) 2011-2013 Phalcon Team (http://www.phalconphp.com)       |
 +------------------------------------------------------------------------+
 | This source file is subject to the New BSD License that is bundled     |
 | with this package in the file docs/LICENSE.txt.                        |
 |                                                                        |
 | If you did not receive a copy of the license and are unable to         |
 | obtain it through the world-wide-web, please send an email             |
 | to license@phalconphp.com so we can send you a copy immediately.       |
 +------------------------------------------------------------------------+
 | Authors: Andres Gutierrez <andres@phalconphp.com>                      |
 |          Eduar Carvajal <eduar@phalconphp.com>                         |
 |          Kenji Minamoto <kenji.minamoto@gmail.com>                     |
 +------------------------------------------------------------------------+
 */
/**
 * Phalcon\Mvc\Model\Query
 *
 * This class takes a PHQL intermediate representation and executes it.
 *
 *<code>
 *
 * $phql = "SELECT c.price*0.16 AS taxes, c.* FROM Cars AS c JOIN Brands AS b
 *          WHERE b.name = :name: ORDER BY c.name";
 *
 * $result = manager->executeQuery($phql, array(
 *   "name": "Lamborghini"
 * ));
 *
 * foreach ($result as $row) {
 *   echo "Name: ", $row->cars->name, "\n";
 *   echo "Price: ", $row->cars->price, "\n";
 *   echo "Taxes: ", $row->taxes, "\n";
 * }
 *
 *</code>
 */
ZEPHIR_INIT_CLASS(Phalcon_Mvc_Model_Query) {

	ZEPHIR_REGISTER_CLASS(Phalcon\\Mvc\\Model, Query, phalcon, mvc_model_query, phalcon_mvc_model_query_method_entry, 0);

	zend_declare_property_null(phalcon_mvc_model_query_ce, SL("_dependencyInjector"), ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(phalcon_mvc_model_query_ce, SL("_manager"), ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(phalcon_mvc_model_query_ce, SL("_metaData"), ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(phalcon_mvc_model_query_ce, SL("_type"), ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(phalcon_mvc_model_query_ce, SL("_phql"), ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(phalcon_mvc_model_query_ce, SL("_ast"), ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(phalcon_mvc_model_query_ce, SL("_intermediate"), ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(phalcon_mvc_model_query_ce, SL("_models"), ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(phalcon_mvc_model_query_ce, SL("_sqlAliases"), ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(phalcon_mvc_model_query_ce, SL("_sqlAliasesModels"), ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(phalcon_mvc_model_query_ce, SL("_sqlModelsAliases"), ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(phalcon_mvc_model_query_ce, SL("_sqlAliasesModelsInstances"), ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(phalcon_mvc_model_query_ce, SL("_sqlColumnAliases"), ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(phalcon_mvc_model_query_ce, SL("_modelsInstances"), ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(phalcon_mvc_model_query_ce, SL("_cache"), ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(phalcon_mvc_model_query_ce, SL("_cacheOptions"), ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(phalcon_mvc_model_query_ce, SL("_uniqueRow"), ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(phalcon_mvc_model_query_ce, SL("_bindParams"), ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(phalcon_mvc_model_query_ce, SL("_bindTypes"), ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(phalcon_mvc_model_query_ce, SL("_irPhqlCache"), ZEND_ACC_STATIC|ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_class_constant_long(phalcon_mvc_model_query_ce, SL("TYPE_SELECT"), 309 TSRMLS_CC);
	zend_declare_class_constant_long(phalcon_mvc_model_query_ce, SL("TYPE_INSERT"), 306 TSRMLS_CC);
	zend_declare_class_constant_long(phalcon_mvc_model_query_ce, SL("TYPE_UPDATE"), 300 TSRMLS_CC);
	zend_declare_class_constant_long(phalcon_mvc_model_query_ce, SL("TYPE_DELETE"), 303 TSRMLS_CC);

	return SUCCESS;

}

/**
 * Sets the dependency injection container
 *
 * @param Phalcon\DiInterface dependencyInjector
 */
PHP_METHOD(Phalcon_Mvc_Model_Query, setDI) {

	zval *dependencyInjector, *manager, *metaData, *_0;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &dependencyInjector);



	ZEPHIR_INIT_VAR(_0);
	ZVAL_STRING(_0, "modelsManager", 1);
	ZEPHIR_INIT_VAR(manager);
	zephir_call_method_p1(manager, dependencyInjector, "getshared", _0);
	if ((Z_TYPE_P(manager) != IS_OBJECT)) {
		ZEPHIR_THROW_EXCEPTION_STR(phalcon_mvc_model_exception_ce, "Injected service 'modelsManager' is invalid");
		return;
	}
	ZEPHIR_INIT_BNVAR(_0);
	ZVAL_STRING(_0, "modelsMetadata", 1);
	ZEPHIR_INIT_VAR(metaData);
	zephir_call_method_p1(metaData, dependencyInjector, "getshared", _0);
	if ((Z_TYPE_P(metaData) != IS_OBJECT)) {
		ZEPHIR_THROW_EXCEPTION_STR(phalcon_mvc_model_exception_ce, "Injected service 'modelsMetaData' is invalid");
		return;
	}
	zephir_update_property_this(this_ptr, SL("_manager"), manager TSRMLS_CC);
	zephir_update_property_this(this_ptr, SL("_metaData"), metaData TSRMLS_CC);
	zephir_update_property_this(this_ptr, SL("_dependencyInjector"), dependencyInjector TSRMLS_CC);
	ZEPHIR_MM_RESTORE();

}

/**
 * Returns the dependency injection container
 *
 * @return Phalcon\DiInterface
 */
PHP_METHOD(Phalcon_Mvc_Model_Query, getDI) {


	RETURN_MEMBER(this_ptr, "_dependencyInjector");

}

/**
 * Tells to the query if only the first row in the resultset must be returned
 *
 * @param boolean uniqueRow
 * @return Phalcon\Mvc\Model\Query
 */
PHP_METHOD(Phalcon_Mvc_Model_Query, setUniqueRow) {

	zval *uniqueRow_param = NULL;
	zend_bool uniqueRow;

	zephir_fetch_params(0, 1, 0, &uniqueRow_param);

		uniqueRow = zephir_get_boolval(uniqueRow_param);


	zephir_update_property_this(this_ptr, SL("_uniqueRow"), uniqueRow ? ZEPHIR_GLOBAL(global_true) : ZEPHIR_GLOBAL(global_false) TSRMLS_CC);
	RETURN_THISW();

}

/**
 * Check if the query is programmed to get only the first row in the resultset
 *
 * @return boolean
 */
PHP_METHOD(Phalcon_Mvc_Model_Query, getUniqueRow) {


	RETURN_MEMBER(this_ptr, "_uniqueRow");

}

/**
 * Replaces the model's name to its source name in a qualifed-name expression
 *
 * @param array expr
 * @return string
 */
PHP_METHOD(Phalcon_Mvc_Model_Query, _getQualified) {

	zend_function *_6 = NULL, *_9 = NULL;
	HashTable *_4;
	HashPosition _3;
	int number;
	zend_bool hasModel;
	zval *expr, *columnName, *sqlColumnAliases, *metaData, *sqlAliases, *source = NULL, *sqlAliasesModelsInstances, *realColumnName = NULL, *columnDomain, *model = NULL, *models, *columnMap = NULL, *_0 = NULL, *_1, *_2 = NULL, **_5, *_7 = NULL, *_8;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &expr);



	zephir_array_fetch_string(&columnName, expr, SL("name"), PH_NOISY | PH_READONLY TSRMLS_CC);
	sqlColumnAliases = zephir_fetch_nproperty_this(this_ptr, SL("_sqlColumnAliases"), PH_NOISY_CC);
	if (zephir_array_isset(sqlColumnAliases, columnName)) {
		array_init(return_value);
		add_assoc_stringl_ex(return_value, SS("type"), SL("qualified"), 1);
		zephir_array_update_string(&return_value, SL("name"), &columnName, PH_COPY | PH_SEPARATE);
		RETURN_MM();
	}
	metaData = zephir_fetch_nproperty_this(this_ptr, SL("_metaData"), PH_NOISY_CC);
	if (zephir_array_isset_string(expr, SS("domain"))) {
		sqlAliases = zephir_fetch_nproperty_this(this_ptr, SL("_sqlAliases"), PH_NOISY_CC);
		zephir_array_fetch_string(&columnDomain, expr, SL("domain"), PH_NOISY | PH_READONLY TSRMLS_CC);
		ZEPHIR_OBS_VAR(source);
		if (!(zephir_array_isset_fetch(&source, sqlAliases, columnDomain, 0 TSRMLS_CC))) {
			ZEPHIR_INIT_VAR(_0);
			object_init_ex(_0, phalcon_mvc_model_exception_ce);
			_1 = zephir_fetch_nproperty_this(this_ptr, SL("_phql"), PH_NOISY_CC);
			ZEPHIR_INIT_VAR(_2);
			ZEPHIR_CONCAT_SVSV(_2, "Unknown model or alias '", columnDomain, "' (1), when preparing: ", _1);
			zephir_call_method_p1_noret(_0, "__construct", _2);
			zephir_throw_exception(_0 TSRMLS_CC);
			ZEPHIR_MM_RESTORE();
			return;
		}
		ZEPHIR_INIT_VAR(columnMap);
		if (ZEPHIR_GLOBAL(orm).column_renaming) {
			sqlAliasesModelsInstances = zephir_fetch_nproperty_this(this_ptr, SL("_sqlAliasesModelsInstances"), PH_NOISY_CC);
			ZEPHIR_OBS_VAR(model);
			if (!(zephir_array_isset_fetch(&model, sqlAliasesModelsInstances, columnDomain, 0 TSRMLS_CC))) {
				ZEPHIR_INIT_NVAR(_0);
				object_init_ex(_0, phalcon_mvc_model_exception_ce);
				_1 = zephir_fetch_nproperty_this(this_ptr, SL("_phql"), PH_NOISY_CC);
				ZEPHIR_INIT_LNVAR(_2);
				ZEPHIR_CONCAT_SVSV(_2, "There is no model related to model or alias '", columnDomain, "', when executing: ", _1);
				zephir_call_method_p1_noret(_0, "__construct", _2);
				zephir_throw_exception(_0 TSRMLS_CC);
				ZEPHIR_MM_RESTORE();
				return;
			}
			zephir_call_method_p1(columnMap, metaData, "getreversecolumnmap", model);
		} else {
			ZVAL_NULL(columnMap);
		}
		if ((Z_TYPE_P(columnMap) == IS_ARRAY)) {
			ZEPHIR_OBS_VAR(realColumnName);
			if (!(zephir_array_isset_fetch(&realColumnName, columnMap, columnName, 0 TSRMLS_CC))) {
				ZEPHIR_INIT_NVAR(_0);
				object_init_ex(_0, phalcon_mvc_model_exception_ce);
				_1 = zephir_fetch_nproperty_this(this_ptr, SL("_phql"), PH_NOISY_CC);
				ZEPHIR_INIT_LNVAR(_2);
				ZEPHIR_CONCAT_SVSVSV(_2, "Column '", columnName, "' doesn't belong to the model or alias '", columnDomain, "', when executing: ", _1);
				zephir_call_method_p1_noret(_0, "__construct", _2);
				zephir_throw_exception(_0 TSRMLS_CC);
				ZEPHIR_MM_RESTORE();
				return;
			}
		} else {
			ZEPHIR_CPY_WRT(realColumnName, columnName);
		}
	} else {
		number = 0;
		hasModel = 0;
		_1 = zephir_fetch_nproperty_this(this_ptr, SL("_modelsInstances"), PH_NOISY_CC);
		zephir_is_iterable(_1, &_4, &_3, 0, 0);
		for (
			; zend_hash_get_current_data_ex(_4, (void**) &_5, &_3) == SUCCESS
			; zend_hash_move_forward_ex(_4, &_3)
		) {
			ZEPHIR_GET_HVALUE(model, _5);
			ZEPHIR_INIT_NVAR(_0);
			zephir_call_method_p2_cache(_0, metaData, "hasattribute", &_6, model, columnName);
			if (zephir_is_true(_0)) {
				number++;
				if ((number > 1)) {
					ZEPHIR_INIT_NVAR(_7);
					object_init_ex(_7, phalcon_mvc_model_exception_ce);
					_8 = zephir_fetch_nproperty_this(this_ptr, SL("_phql"), PH_NOISY_CC);
					ZEPHIR_INIT_LNVAR(_2);
					ZEPHIR_CONCAT_SVSV(_2, "The column '", columnName, "' is ambiguous, when preparing: ", _8);
					zephir_call_method_p1_cache_noret(_7, "__construct", &_9, _2);
					zephir_throw_exception(_7 TSRMLS_CC);
					ZEPHIR_MM_RESTORE();
					return;
				}
				hasModel = zephir_get_boolval(model);
			}
		}
		if ((hasModel == 0)) {
			ZEPHIR_INIT_NVAR(_0);
			object_init_ex(_0, phalcon_mvc_model_exception_ce);
			_1 = zephir_fetch_nproperty_this(this_ptr, SL("_phql"), PH_NOISY_CC);
			ZEPHIR_INIT_LNVAR(_2);
			ZEPHIR_CONCAT_SVSV(_2, "Column '", columnName, "' doesn't belong to any of the selected models (1), when preparing: ", _1);
			zephir_call_method_p1_noret(_0, "__construct", _2);
			zephir_throw_exception(_0 TSRMLS_CC);
			ZEPHIR_MM_RESTORE();
			return;
		}
		models = zephir_fetch_nproperty_this(this_ptr, SL("_models"), PH_NOISY_CC);
		if ((Z_TYPE_P(models) != IS_ARRAY)) {
			ZEPHIR_THROW_EXCEPTION_STR(phalcon_mvc_model_exception_ce, "The models list was not loaded correctly");
			return;
		}
		ZEPHIR_OBS_NVAR(source);
		ZEPHIR_INIT_NVAR(_0);
		zephir_call_func_p1(_0, "get_class", (hasModel ? ZEPHIR_GLOBAL(global_true) : ZEPHIR_GLOBAL(global_false)));
		if (!(zephir_array_isset_fetch(&source, models, _0, 0 TSRMLS_CC))) {
			ZEPHIR_INIT_NVAR(_7);
			object_init_ex(_7, phalcon_mvc_model_exception_ce);
			_1 = zephir_fetch_nproperty_this(this_ptr, SL("_phql"), PH_NOISY_CC);
			ZEPHIR_INIT_LNVAR(_2);
			ZEPHIR_CONCAT_SVSV(_2, "Column '", columnName, "' doesn't belong to any of the selected models (2), when preparing: ", _1);
			zephir_call_method_p1_noret(_7, "__construct", _2);
			zephir_throw_exception(_7 TSRMLS_CC);
			ZEPHIR_MM_RESTORE();
			return;
		}
		ZEPHIR_INIT_NVAR(columnMap);
		if (ZEPHIR_GLOBAL(orm).column_renaming) {
			zephir_call_method_p1(columnMap, metaData, "getreversecolumnmap", (hasModel ? ZEPHIR_GLOBAL(global_true) : ZEPHIR_GLOBAL(global_false)));
		} else {
			ZVAL_NULL(columnMap);
		}
		if ((Z_TYPE_P(columnMap) == IS_ARRAY)) {
			ZEPHIR_OBS_NVAR(realColumnName);
			if (!(zephir_array_isset_fetch(&realColumnName, columnMap, columnName, 0 TSRMLS_CC))) {
				ZEPHIR_INIT_NVAR(_0);
				object_init_ex(_0, phalcon_mvc_model_exception_ce);
				_1 = zephir_fetch_nproperty_this(this_ptr, SL("_phql"), PH_NOISY_CC);
				ZEPHIR_INIT_LNVAR(_2);
				ZEPHIR_CONCAT_SVSV(_2, "Column '", columnName, "' doesn't belong to any of the selected models (3), when preparing: ", _1);
				zephir_call_method_p1_noret(_0, "__construct", _2);
				zephir_throw_exception(_0 TSRMLS_CC);
				ZEPHIR_MM_RESTORE();
				return;
			}
		} else {
			ZEPHIR_CPY_WRT(realColumnName, columnName);
		}
	}
	array_init(return_value);
	add_assoc_stringl_ex(return_value, SS("type"), SL("qualified"), 1);
	zephir_array_update_string(&return_value, SL("domain"), &source, PH_COPY | PH_SEPARATE);
	zephir_array_update_string(&return_value, SL("name"), &realColumnName, PH_COPY | PH_SEPARATE);
	zephir_array_update_string(&return_value, SL("balias"), &columnName, PH_COPY | PH_SEPARATE);
	RETURN_MM();

}

/**
 * Resolves a expression in a single call argument
 *
 * @param array argument
 * @return string
 */
PHP_METHOD(Phalcon_Mvc_Model_Query, _getCallArgument) {

	zval *argument, *_0;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &argument);



	zephir_array_fetch_string(&_0, argument, SL("type"), PH_NOISY | PH_READONLY TSRMLS_CC);
	if (ZEPHIR_IS_LONG(_0, 352)) {
		array_init(return_value);
		add_assoc_stringl_ex(return_value, SS("type"), SL("all"), 1);
		RETURN_MM();
	}
	zephir_call_method_p1(return_value, this_ptr, "_getexpression", argument);
	RETURN_MM();

}

/**
 * Resolves a expression in a single call argument
 *
 * @param array expr
 * @return array
 */
PHP_METHOD(Phalcon_Mvc_Model_Query, _getFunctionCall) {

	zend_function *_4 = NULL;
	HashTable *_1;
	HashPosition _0;
	zval *expr, *arguments, *argument = NULL, *functionArgs, **_2, *_3 = NULL, *_5 = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &expr);



	if (zephir_array_isset_string_fetch(&arguments, expr, SS("arguments"), 1 TSRMLS_CC)) {
		ZEPHIR_INIT_VAR(functionArgs);
		if (zephir_array_isset_long(arguments, 0)) {
			array_init(functionArgs);
			zephir_is_iterable(arguments, &_1, &_0, 0, 0);
			for (
				; zend_hash_get_current_data_ex(_1, (void**) &_2, &_0) == SUCCESS
				; zend_hash_move_forward_ex(_1, &_0)
			) {
				ZEPHIR_GET_HVALUE(argument, _2);
				ZEPHIR_INIT_NVAR(_3);
				zephir_call_method_p1_cache(_3, this_ptr, "_getcallargument", &_4, argument);
				zephir_array_append(&functionArgs, _3, PH_SEPARATE);
			}
		} else {
			array_init(functionArgs);
			ZEPHIR_INIT_NVAR(_3);
			zephir_call_method_p1(_3, this_ptr, "_getcallargument", arguments);
			zephir_array_fast_append(functionArgs, _3);
		}
		array_init(return_value);
		add_assoc_stringl_ex(return_value, SS("type"), SL("functionCall"), 1);
		ZEPHIR_OBS_VAR(_5);
		zephir_array_fetch_string(&_5, expr, SL("name"), PH_NOISY TSRMLS_CC);
		zephir_array_update_string(&return_value, SL("name"), &_5, PH_COPY | PH_SEPARATE);
		zephir_array_update_string(&return_value, SL("arguments"), &functionArgs, PH_COPY | PH_SEPARATE);
		RETURN_MM();
	}
	array_init(return_value);
	add_assoc_stringl_ex(return_value, SS("type"), SL("functionCall"), 1);
	ZEPHIR_OBS_NVAR(_5);
	zephir_array_fetch_string(&_5, expr, SL("name"), PH_NOISY TSRMLS_CC);
	zephir_array_update_string(&return_value, SL("name"), &_5, PH_COPY | PH_SEPARATE);
	RETURN_MM();

}

/**
 * Resolves an expression from its intermediate code into a string
 *
 * @param array expr
 * @param boolean quoting
 * @return string
 */
PHP_METHOD(Phalcon_Mvc_Model_Query, _getExpression) {

	zend_function *_9 = NULL;
	HashTable *_7;
	HashPosition _6;
	zend_bool quoting, tempNotQuoting;
	zval *expr, *quoting_param = NULL, *exprType, *exprLeft, *exprRight, *left, *right, *listItems, *exprListItem = NULL, *exprReturn = NULL, *value, *escapedValue = NULL, *exprValue = NULL, *_0 = NULL, *_1, _2, _3, *_4 = NULL, *_5 = NULL, **_8;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 1, &expr, &quoting_param);

	if (!quoting_param) {
		quoting = 1;
	} else {
		quoting = zephir_get_boolval(quoting_param);
	}


	if (zephir_array_isset_string_fetch(&exprType, expr, SS("type"), 1 TSRMLS_CC)) {
		tempNotQuoting = 1;
		if (zephir_array_isset_string_fetch(&exprLeft, expr, SS("left"), 1 TSRMLS_CC)) {
			ZEPHIR_INIT_VAR(left);
			zephir_call_method_p2(left, this_ptr, "_getexpression", exprLeft, (tempNotQuoting ? ZEPHIR_GLOBAL(global_true) : ZEPHIR_GLOBAL(global_false)));
		}
		if (zephir_array_isset_string_fetch(&exprRight, expr, SS("right"), 1 TSRMLS_CC)) {
			ZEPHIR_INIT_VAR(right);
			zephir_call_method_p2(right, this_ptr, "_getexpression", exprRight, (tempNotQuoting ? ZEPHIR_GLOBAL(global_true) : ZEPHIR_GLOBAL(global_false)));
		}
		do {
			if (ZEPHIR_IS_LONG(exprType, '<')) {
				ZEPHIR_INIT_VAR(exprReturn);
				array_init(exprReturn);
				add_assoc_stringl_ex(exprReturn, SS("type"), SL("binary-op"), 1);
				add_assoc_stringl_ex(exprReturn, SS("op"), SL("<"), 1);
				zephir_array_update_string(&exprReturn, SL("left"), &left, PH_COPY | PH_SEPARATE);
				zephir_array_update_string(&exprReturn, SL("right"), &right, PH_COPY | PH_SEPARATE);
				break;
			}
			if (ZEPHIR_IS_LONG(exprType, '=')) {
				ZEPHIR_INIT_NVAR(exprReturn);
				array_init(exprReturn);
				add_assoc_stringl_ex(exprReturn, SS("type"), SL("binary-op"), 1);
				add_assoc_stringl_ex(exprReturn, SS("op"), SL("="), 1);
				zephir_array_update_string(&exprReturn, SL("left"), &left, PH_COPY | PH_SEPARATE);
				zephir_array_update_string(&exprReturn, SL("right"), &right, PH_COPY | PH_SEPARATE);
				break;
			}
			if (ZEPHIR_IS_LONG(exprType, '>')) {
				ZEPHIR_INIT_NVAR(exprReturn);
				array_init(exprReturn);
				add_assoc_stringl_ex(exprReturn, SS("type"), SL("binary-op"), 1);
				add_assoc_stringl_ex(exprReturn, SS("op"), SL(">"), 1);
				zephir_array_update_string(&exprReturn, SL("left"), &left, PH_COPY | PH_SEPARATE);
				zephir_array_update_string(&exprReturn, SL("right"), &right, PH_COPY | PH_SEPARATE);
				break;
			}
			if (ZEPHIR_IS_LONG(exprType, 270)) {
				ZEPHIR_INIT_NVAR(exprReturn);
				array_init(exprReturn);
				add_assoc_stringl_ex(exprReturn, SS("type"), SL("binary-op"), 1);
				add_assoc_stringl_ex(exprReturn, SS("op"), SL("<>"), 1);
				zephir_array_update_string(&exprReturn, SL("left"), &left, PH_COPY | PH_SEPARATE);
				zephir_array_update_string(&exprReturn, SL("right"), &right, PH_COPY | PH_SEPARATE);
				break;
			}
			if (ZEPHIR_IS_LONG(exprType, 271)) {
				ZEPHIR_INIT_NVAR(exprReturn);
				array_init(exprReturn);
				add_assoc_stringl_ex(exprReturn, SS("type"), SL("binary-op"), 1);
				add_assoc_stringl_ex(exprReturn, SS("op"), SL("<="), 1);
				zephir_array_update_string(&exprReturn, SL("left"), &left, PH_COPY | PH_SEPARATE);
				zephir_array_update_string(&exprReturn, SL("right"), &right, PH_COPY | PH_SEPARATE);
				break;
			}
			if (ZEPHIR_IS_LONG(exprType, 272)) {
				ZEPHIR_INIT_NVAR(exprReturn);
				array_init(exprReturn);
				add_assoc_stringl_ex(exprReturn, SS("type"), SL("binary-op"), 1);
				add_assoc_stringl_ex(exprReturn, SS("op"), SL(">="), 1);
				zephir_array_update_string(&exprReturn, SL("left"), &left, PH_COPY | PH_SEPARATE);
				zephir_array_update_string(&exprReturn, SL("right"), &right, PH_COPY | PH_SEPARATE);
				break;
			}
			if (ZEPHIR_IS_LONG(exprType, 266)) {
				ZEPHIR_INIT_NVAR(exprReturn);
				array_init(exprReturn);
				add_assoc_stringl_ex(exprReturn, SS("type"), SL("binary-op"), 1);
				add_assoc_stringl_ex(exprReturn, SS("op"), SL("AND"), 1);
				zephir_array_update_string(&exprReturn, SL("left"), &left, PH_COPY | PH_SEPARATE);
				zephir_array_update_string(&exprReturn, SL("right"), &right, PH_COPY | PH_SEPARATE);
				break;
			}
			if (ZEPHIR_IS_LONG(exprType, 267)) {
				ZEPHIR_INIT_NVAR(exprReturn);
				array_init(exprReturn);
				add_assoc_stringl_ex(exprReturn, SS("type"), SL("binary-op"), 1);
				add_assoc_stringl_ex(exprReturn, SS("op"), SL("OR"), 1);
				zephir_array_update_string(&exprReturn, SL("left"), &left, PH_COPY | PH_SEPARATE);
				zephir_array_update_string(&exprReturn, SL("right"), &right, PH_COPY | PH_SEPARATE);
				break;
			}
			if (ZEPHIR_IS_LONG(exprType, 355)) {
				ZEPHIR_INIT_NVAR(exprReturn);
				zephir_call_method_p1(exprReturn, this_ptr, "_getqualified", expr);
				break;
			}
			if (ZEPHIR_IS_LONG(exprType, 359)) {
				break;
			}
			if (ZEPHIR_IS_LONG(exprType, '+')) {
				ZEPHIR_INIT_NVAR(exprReturn);
				array_init(exprReturn);
				add_assoc_stringl_ex(exprReturn, SS("type"), SL("binary-op"), 1);
				add_assoc_stringl_ex(exprReturn, SS("op"), SL("+"), 1);
				zephir_array_update_string(&exprReturn, SL("left"), &left, PH_COPY | PH_SEPARATE);
				zephir_array_update_string(&exprReturn, SL("right"), &right, PH_COPY | PH_SEPARATE);
				break;
			}
			if (ZEPHIR_IS_LONG(exprType, '-')) {
				ZEPHIR_INIT_NVAR(exprReturn);
				array_init(exprReturn);
				add_assoc_stringl_ex(exprReturn, SS("type"), SL("binary-op"), 1);
				add_assoc_stringl_ex(exprReturn, SS("op"), SL("-"), 1);
				zephir_array_update_string(&exprReturn, SL("left"), &left, PH_COPY | PH_SEPARATE);
				zephir_array_update_string(&exprReturn, SL("right"), &right, PH_COPY | PH_SEPARATE);
				break;
			}
			if (ZEPHIR_IS_LONG(exprType, '*')) {
				ZEPHIR_INIT_NVAR(exprReturn);
				array_init(exprReturn);
				add_assoc_stringl_ex(exprReturn, SS("type"), SL("binary-op"), 1);
				add_assoc_stringl_ex(exprReturn, SS("op"), SL("*"), 1);
				zephir_array_update_string(&exprReturn, SL("left"), &left, PH_COPY | PH_SEPARATE);
				zephir_array_update_string(&exprReturn, SL("right"), &right, PH_COPY | PH_SEPARATE);
				break;
			}
			if (ZEPHIR_IS_LONG(exprType, '/')) {
				ZEPHIR_INIT_NVAR(exprReturn);
				array_init(exprReturn);
				add_assoc_stringl_ex(exprReturn, SS("type"), SL("binary-op"), 1);
				add_assoc_stringl_ex(exprReturn, SS("op"), SL("/"), 1);
				zephir_array_update_string(&exprReturn, SL("left"), &left, PH_COPY | PH_SEPARATE);
				zephir_array_update_string(&exprReturn, SL("right"), &right, PH_COPY | PH_SEPARATE);
				break;
			}
			if (ZEPHIR_IS_LONG(exprType, '%')) {
				ZEPHIR_INIT_NVAR(exprReturn);
				array_init(exprReturn);
				add_assoc_stringl_ex(exprReturn, SS("type"), SL("binary-op"), 1);
				add_assoc_stringl_ex(exprReturn, SS("op"), SL("%"), 1);
				zephir_array_update_string(&exprReturn, SL("left"), &left, PH_COPY | PH_SEPARATE);
				zephir_array_update_string(&exprReturn, SL("right"), &right, PH_COPY | PH_SEPARATE);
				break;
			}
			if (ZEPHIR_IS_LONG(exprType, 38)) {
				ZEPHIR_INIT_NVAR(exprReturn);
				array_init(exprReturn);
				add_assoc_stringl_ex(exprReturn, SS("type"), SL("binary-op"), 1);
				add_assoc_stringl_ex(exprReturn, SS("op"), SL("&"), 1);
				zephir_array_update_string(&exprReturn, SL("left"), &left, PH_COPY | PH_SEPARATE);
				zephir_array_update_string(&exprReturn, SL("right"), &right, PH_COPY | PH_SEPARATE);
				break;
			}
			if (ZEPHIR_IS_LONG(exprType, 124)) {
				ZEPHIR_INIT_NVAR(exprReturn);
				array_init(exprReturn);
				add_assoc_stringl_ex(exprReturn, SS("type"), SL("binary-op"), 1);
				add_assoc_stringl_ex(exprReturn, SS("op"), SL("|"), 1);
				zephir_array_update_string(&exprReturn, SL("left"), &left, PH_COPY | PH_SEPARATE);
				zephir_array_update_string(&exprReturn, SL("right"), &right, PH_COPY | PH_SEPARATE);
				break;
			}
			if (ZEPHIR_IS_LONG(exprType, 356)) {
				ZEPHIR_INIT_NVAR(exprReturn);
				array_init(exprReturn);
				add_assoc_stringl_ex(exprReturn, SS("type"), SL("parentheses"), 1);
				zephir_array_update_string(&exprReturn, SL("left"), &left, PH_COPY | PH_SEPARATE);
				break;
			}
			if (ZEPHIR_IS_LONG(exprType, 367)) {
				ZEPHIR_INIT_NVAR(exprReturn);
				array_init(exprReturn);
				add_assoc_stringl_ex(exprReturn, SS("type"), SL("unary-op"), 1);
				add_assoc_stringl_ex(exprReturn, SS("op"), SL("-"), 1);
				zephir_array_update_string(&exprReturn, SL("right"), &right, PH_COPY | PH_SEPARATE);
				break;
			}
			if (ZEPHIR_IS_LONG(exprType, 258) || ZEPHIR_IS_LONG(exprType, 259)) {
				ZEPHIR_INIT_NVAR(exprReturn);
				array_init(exprReturn);
				add_assoc_stringl_ex(exprReturn, SS("type"), SL("literal"), 1);
				ZEPHIR_OBS_VAR(_0);
				zephir_array_fetch_string(&_0, expr, SL("value"), PH_NOISY TSRMLS_CC);
				zephir_array_update_string(&exprReturn, SL("value"), &_0, PH_COPY | PH_SEPARATE);
				break;
			}
			if (ZEPHIR_IS_LONG(exprType, 333)) {
				ZEPHIR_INIT_NVAR(exprReturn);
				array_init(exprReturn);
				add_assoc_stringl_ex(exprReturn, SS("type"), SL("literal"), 1);
				add_assoc_stringl_ex(exprReturn, SS("value"), SL("TRUE"), 1);
				break;
			}
			if (ZEPHIR_IS_LONG(exprType, 334)) {
				ZEPHIR_INIT_NVAR(exprReturn);
				array_init(exprReturn);
				add_assoc_stringl_ex(exprReturn, SS("type"), SL("literal"), 1);
				add_assoc_stringl_ex(exprReturn, SS("value"), SL("FALSE"), 1);
				break;
			}
			if (ZEPHIR_IS_LONG(exprType, 260)) {
				zephir_array_fetch_string(&value, expr, SL("value"), PH_NOISY | PH_READONLY TSRMLS_CC);
				if ((quoting == 1)) {
					if (zephir_memnstr_str(value, SL("'"), "phalcon/mvc/model/query.zep", 460)) {
						ZEPHIR_INIT_VAR(escapedValue);
						zephir_call_func_p1(escapedValue, "phalcon_orm_singlequotes", value);
					} else {
						ZEPHIR_CPY_WRT(escapedValue, value);
					}
					ZEPHIR_INIT_VAR(exprValue);
					ZEPHIR_CONCAT_SVS(exprValue, "'", escapedValue, "'");
				} else {
					ZEPHIR_CPY_WRT(exprValue, value);
				}
				ZEPHIR_INIT_NVAR(exprReturn);
				array_init(exprReturn);
				add_assoc_stringl_ex(exprReturn, SS("type"), SL("literal"), 1);
				zephir_array_update_string(&exprReturn, SL("value"), &exprValue, PH_COPY | PH_SEPARATE);
				break;
			}
			if (ZEPHIR_IS_LONG(exprType, 273)) {
				ZEPHIR_INIT_NVAR(exprReturn);
				array_init(exprReturn);
				add_assoc_stringl_ex(exprReturn, SS("type"), SL("placeholder"), 1);
				zephir_array_fetch_string(&_1, expr, SL("value"), PH_NOISY | PH_READONLY TSRMLS_CC);
				ZEPHIR_SINIT_VAR(_2);
				ZVAL_STRING(&_2, "?", 0);
				ZEPHIR_SINIT_VAR(_3);
				ZVAL_STRING(&_3, ":", 0);
				ZEPHIR_INIT_VAR(_4);
				zephir_call_func_p3(_4, "str_replace", &_2, &_3, _1);
				zephir_array_update_string(&exprReturn, SL("value"), &_4, PH_COPY | PH_SEPARATE);
				break;
			}
			if (ZEPHIR_IS_LONG(exprType, 274)) {
				ZEPHIR_INIT_NVAR(exprReturn);
				array_init(exprReturn);
				add_assoc_stringl_ex(exprReturn, SS("type"), SL("placeholder"), 1);
				zephir_array_fetch_string(&_1, expr, SL("value"), PH_NOISY | PH_READONLY TSRMLS_CC);
				ZEPHIR_INIT_VAR(_5);
				ZEPHIR_CONCAT_SV(_5, ":", _1);
				zephir_array_update_string(&exprReturn, SL("value"), &_5, PH_COPY | PH_SEPARATE);
				break;
			}
			if (ZEPHIR_IS_LONG(exprType, 322)) {
				ZEPHIR_INIT_NVAR(exprReturn);
				array_init(exprReturn);
				add_assoc_stringl_ex(exprReturn, SS("type"), SL("literal"), 1);
				add_assoc_stringl_ex(exprReturn, SS("value"), SL("NULL"), 1);
				break;
			}
			if (ZEPHIR_IS_LONG(exprType, 268)) {
				ZEPHIR_INIT_NVAR(exprReturn);
				array_init(exprReturn);
				add_assoc_stringl_ex(exprReturn, SS("type"), SL("binary-op"), 1);
				add_assoc_stringl_ex(exprReturn, SS("op"), SL("LIKE"), 1);
				zephir_array_update_string(&exprReturn, SL("left"), &left, PH_COPY | PH_SEPARATE);
				zephir_array_update_string(&exprReturn, SL("right"), &right, PH_COPY | PH_SEPARATE);
				break;
			}
			if (ZEPHIR_IS_LONG(exprType, 351)) {
				ZEPHIR_INIT_NVAR(exprReturn);
				array_init(exprReturn);
				add_assoc_stringl_ex(exprReturn, SS("type"), SL("binary-op"), 1);
				add_assoc_stringl_ex(exprReturn, SS("op"), SL("NOT LIKE"), 1);
				zephir_array_update_string(&exprReturn, SL("left"), &left, PH_COPY | PH_SEPARATE);
				zephir_array_update_string(&exprReturn, SL("right"), &right, PH_COPY | PH_SEPARATE);
				break;
			}
			if (ZEPHIR_IS_LONG(exprType, 275)) {
				ZEPHIR_INIT_NVAR(exprReturn);
				array_init(exprReturn);
				add_assoc_stringl_ex(exprReturn, SS("type"), SL("binary-op"), 1);
				add_assoc_stringl_ex(exprReturn, SS("op"), SL("ILIKE"), 1);
				zephir_array_update_string(&exprReturn, SL("left"), &left, PH_COPY | PH_SEPARATE);
				zephir_array_update_string(&exprReturn, SL("right"), &right, PH_COPY | PH_SEPARATE);
				break;
			}
			if (ZEPHIR_IS_LONG(exprType, 357)) {
				ZEPHIR_INIT_NVAR(exprReturn);
				array_init(exprReturn);
				add_assoc_stringl_ex(exprReturn, SS("type"), SL("binary-op"), 1);
				add_assoc_stringl_ex(exprReturn, SS("op"), SL("NOT ILIKE"), 1);
				zephir_array_update_string(&exprReturn, SL("left"), &left, PH_COPY | PH_SEPARATE);
				zephir_array_update_string(&exprReturn, SL("right"), &right, PH_COPY | PH_SEPARATE);
				break;
			}
			if (ZEPHIR_IS_LONG(exprType, '!')) {
				ZEPHIR_INIT_NVAR(exprReturn);
				array_init(exprReturn);
				add_assoc_stringl_ex(exprReturn, SS("type"), SL("unary-op"), 1);
				add_assoc_stringl_ex(exprReturn, SS("op"), SL("NOT "), 1);
				zephir_array_update_string(&exprReturn, SL("right"), &right, PH_COPY | PH_SEPARATE);
				break;
			}
			if (ZEPHIR_IS_LONG(exprType, 365)) {
				ZEPHIR_INIT_NVAR(exprReturn);
				array_init(exprReturn);
				add_assoc_stringl_ex(exprReturn, SS("type"), SL("unary-op"), 1);
				add_assoc_stringl_ex(exprReturn, SS("op"), SL(" IS NULL"), 1);
				zephir_array_update_string(&exprReturn, SL("left"), &left, PH_COPY | PH_SEPARATE);
				break;
			}
			if (ZEPHIR_IS_LONG(exprType, 366)) {
				ZEPHIR_INIT_NVAR(exprReturn);
				array_init(exprReturn);
				add_assoc_stringl_ex(exprReturn, SS("type"), SL("unary-op"), 1);
				add_assoc_stringl_ex(exprReturn, SS("op"), SL(" IS NOT NULL"), 1);
				zephir_array_update_string(&exprReturn, SL("left"), &left, PH_COPY | PH_SEPARATE);
				break;
			}
			if (ZEPHIR_IS_LONG(exprType, 315)) {
				ZEPHIR_INIT_NVAR(exprReturn);
				array_init(exprReturn);
				add_assoc_stringl_ex(exprReturn, SS("type"), SL("binary-op"), 1);
				add_assoc_stringl_ex(exprReturn, SS("op"), SL("IN"), 1);
				zephir_array_update_string(&exprReturn, SL("left"), &left, PH_COPY | PH_SEPARATE);
				zephir_array_update_string(&exprReturn, SL("right"), &right, PH_COPY | PH_SEPARATE);
				break;
			}
			if (ZEPHIR_IS_LONG(exprType, 323)) {
				ZEPHIR_INIT_NVAR(exprReturn);
				array_init(exprReturn);
				add_assoc_stringl_ex(exprReturn, SS("type"), SL("binary-op"), 1);
				add_assoc_stringl_ex(exprReturn, SS("op"), SL("NOT IN"), 1);
				zephir_array_update_string(&exprReturn, SL("left"), &left, PH_COPY | PH_SEPARATE);
				zephir_array_update_string(&exprReturn, SL("right"), &right, PH_COPY | PH_SEPARATE);
				break;
			}
			if (ZEPHIR_IS_LONG(exprType, 330)) {
				ZEPHIR_INIT_NVAR(exprReturn);
				array_init(exprReturn);
				add_assoc_stringl_ex(exprReturn, SS("type"), SL("unary-op"), 1);
				add_assoc_stringl_ex(exprReturn, SS("op"), SL("DISTINCT "), 1);
				zephir_array_update_string(&exprReturn, SL("right"), &right, PH_COPY | PH_SEPARATE);
				break;
			}
			if (ZEPHIR_IS_LONG(exprType, 331)) {
				ZEPHIR_INIT_NVAR(exprReturn);
				array_init(exprReturn);
				add_assoc_stringl_ex(exprReturn, SS("type"), SL("binary-op"), 1);
				add_assoc_stringl_ex(exprReturn, SS("op"), SL("BETWEEN"), 1);
				zephir_array_update_string(&exprReturn, SL("left"), &left, PH_COPY | PH_SEPARATE);
				zephir_array_update_string(&exprReturn, SL("right"), &right, PH_COPY | PH_SEPARATE);
				break;
			}
			if (ZEPHIR_IS_LONG(exprType, 276)) {
				ZEPHIR_INIT_NVAR(exprReturn);
				array_init(exprReturn);
				add_assoc_stringl_ex(exprReturn, SS("type"), SL("binary-op"), 1);
				add_assoc_stringl_ex(exprReturn, SS("op"), SL("AGAINST"), 1);
				zephir_array_update_string(&exprReturn, SL("left"), &left, PH_COPY | PH_SEPARATE);
				zephir_array_update_string(&exprReturn, SL("right"), &right, PH_COPY | PH_SEPARATE);
				break;
			}
			if (ZEPHIR_IS_LONG(exprType, 332)) {
				ZEPHIR_INIT_NVAR(exprReturn);
				array_init(exprReturn);
				add_assoc_stringl_ex(exprReturn, SS("type"), SL("cast"), 1);
				zephir_array_update_string(&exprReturn, SL("left"), &left, PH_COPY | PH_SEPARATE);
				zephir_array_update_string(&exprReturn, SL("right"), &right, PH_COPY | PH_SEPARATE);
				break;
			}
			if (ZEPHIR_IS_LONG(exprType, 335)) {
				ZEPHIR_INIT_NVAR(exprReturn);
				array_init(exprReturn);
				add_assoc_stringl_ex(exprReturn, SS("type"), SL("convert"), 1);
				zephir_array_update_string(&exprReturn, SL("left"), &left, PH_COPY | PH_SEPARATE);
				zephir_array_update_string(&exprReturn, SL("right"), &right, PH_COPY | PH_SEPARATE);
				break;
			}
			if (ZEPHIR_IS_LONG(exprType, 358)) {
				ZEPHIR_INIT_NVAR(exprReturn);
				array_init(exprReturn);
				add_assoc_stringl_ex(exprReturn, SS("type"), SL("literal"), 1);
				ZEPHIR_OBS_NVAR(_0);
				zephir_array_fetch_string(&_0, expr, SL("name"), PH_NOISY TSRMLS_CC);
				zephir_array_update_string(&exprReturn, SL("value"), &_0, PH_COPY | PH_SEPARATE);
				break;
			}
			if (ZEPHIR_IS_LONG(exprType, 350)) {
				ZEPHIR_INIT_NVAR(exprReturn);
				zephir_call_method_p1(exprReturn, this_ptr, "_getfunctioncall", expr);
				break;
			}
			ZEPHIR_INIT_NVAR(_4);
			object_init_ex(_4, phalcon_mvc_model_exception_ce);
			ZEPHIR_INIT_LNVAR(_5);
			ZEPHIR_CONCAT_SV(_5, "Unknown expression type ", exprType);
			zephir_call_method_p1_noret(_4, "__construct", _5);
			zephir_throw_exception(_4 TSRMLS_CC);
			ZEPHIR_MM_RESTORE();
			return;
		} while(0);

		RETURN_CCTOR(exprReturn);
	}
	if (zephir_array_isset_string(expr, SS("domain"))) {
		zephir_call_method_p1(return_value, this_ptr, "_getqualified", expr);
		RETURN_MM();
	}
	if (zephir_array_isset_long(expr, 0)) {
		ZEPHIR_INIT_VAR(listItems);
		array_init(listItems);
		zephir_is_iterable(expr, &_7, &_6, 0, 0);
		for (
			; zend_hash_get_current_data_ex(_7, (void**) &_8, &_6) == SUCCESS
			; zend_hash_move_forward_ex(_7, &_6)
		) {
			ZEPHIR_GET_HVALUE(exprListItem, _8);
			ZEPHIR_INIT_NVAR(_4);
			zephir_call_method_p1_cache(_4, this_ptr, "_getexpression", &_9, exprListItem);
			zephir_array_append(&listItems, _4, PH_SEPARATE);
		}
		array_init(return_value);
		add_assoc_stringl_ex(return_value, SS("type"), SL("list"), 1);
		zephir_array_fast_append(return_value, listItems);
		RETURN_MM();
	}
	ZEPHIR_THROW_EXCEPTION_STR(phalcon_mvc_model_exception_ce, "Unknown expression");
	return;

}

/**
 * Resolves a column from its intermediate representation into an array used to determine
 * if the resulset produced is simple or complex
 *
 * @param array column
 * @return array
 */
PHP_METHOD(Phalcon_Mvc_Model_Query, _getSelectColumn) {

	HashTable *_2;
	HashPosition _1;
	zval *column, *sqlColumns, *columnType, *sqlAliases, *modelName = NULL, *source = NULL, *columnDomain, *sqlColumnAlias = NULL, *bestAlias, *preparedAlias = NULL, *sqlExprColumn, *sqlAliasesModels, *sqlModelsAliases, *sqlColumn, *columnData, *balias, *_0, **_3, *_4 = NULL, *_5 = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &column);



	if (!(zephir_array_isset_string_fetch(&columnType, column, SS("type"), 1 TSRMLS_CC))) {
		ZEPHIR_THROW_EXCEPTION_STR(phalcon_mvc_model_exception_ce, "Corrupted SELECT AST");
		return;
	}
	ZEPHIR_INIT_VAR(sqlColumns);
	array_init(sqlColumns);
	if (ZEPHIR_IS_LONG(columnType, 352)) {
		_0 = zephir_fetch_nproperty_this(this_ptr, SL("_models"), PH_NOISY_CC);
		zephir_is_iterable(_0, &_2, &_1, 0, 0);
		for (
			; zend_hash_get_current_data_ex(_2, (void**) &_3, &_1) == SUCCESS
			; zend_hash_move_forward_ex(_2, &_1)
		) {
			ZEPHIR_GET_HMKEY(modelName, _2, _1);
			ZEPHIR_GET_HVALUE(source, _3);
			ZEPHIR_INIT_NVAR(_4);
			array_init(_4);
			add_assoc_stringl_ex(_4, SS("type"), SL("object"), 1);
			zephir_array_update_string(&_4, SL("model"), &modelName, PH_COPY | PH_SEPARATE);
			zephir_array_update_string(&_4, SL("column"), &source, PH_COPY | PH_SEPARATE);
			zephir_array_append(&sqlColumns, _4, PH_SEPARATE);
		}
		RETURN_CCTOR(sqlColumns);
	}
	if (!(zephir_array_isset_string(column, SS("column")))) {
		ZEPHIR_THROW_EXCEPTION_STR(phalcon_mvc_model_exception_ce, "Corrupted SELECT AST");
		return;
	}
	if (ZEPHIR_IS_LONG(columnType, 353)) {
		sqlAliases = zephir_fetch_nproperty_this(this_ptr, SL("_sqlAliases"), PH_NOISY_CC);
		zephir_array_fetch_string(&columnDomain, column, SL("column"), PH_NOISY | PH_READONLY TSRMLS_CC);
		ZEPHIR_OBS_VAR(source);
		if (!(zephir_array_isset_fetch(&source, sqlAliases, columnDomain, 0 TSRMLS_CC))) {
			ZEPHIR_INIT_NVAR(_4);
			object_init_ex(_4, phalcon_mvc_model_exception_ce);
			_0 = zephir_fetch_nproperty_this(this_ptr, SL("_phql"), PH_NOISY_CC);
			ZEPHIR_INIT_VAR(_5);
			ZEPHIR_CONCAT_SVSV(_5, "Unknown model or alias '", columnDomain, "' (2), when preparing: ", _0);
			zephir_call_method_p1_noret(_4, "__construct", _5);
			zephir_throw_exception(_4 TSRMLS_CC);
			ZEPHIR_MM_RESTORE();
			return;
		}
		ZEPHIR_CPY_WRT(sqlColumnAlias, source);
		sqlAliasesModels = zephir_fetch_nproperty_this(this_ptr, SL("_sqlAliasesModels"), PH_NOISY_CC);
		ZEPHIR_OBS_VAR(modelName);
		zephir_array_fetch(&modelName, sqlAliasesModels, columnDomain, PH_NOISY TSRMLS_CC);
		sqlModelsAliases = zephir_fetch_nproperty_this(this_ptr, SL("_sqlModelsAliases"), PH_NOISY_CC);
		zephir_array_fetch(&bestAlias, sqlModelsAliases, modelName, PH_NOISY | PH_READONLY TSRMLS_CC);
		if (ZEPHIR_IS_EQUAL(bestAlias, modelName)) {
			ZEPHIR_INIT_VAR(preparedAlias);
			zephir_call_func_p1(preparedAlias, "lcfirst", modelName);
		} else {
			ZEPHIR_CPY_WRT(preparedAlias, bestAlias);
		}
		ZEPHIR_INIT_NVAR(_4);
		array_init(_4);
		add_assoc_stringl_ex(_4, SS("type"), SL("object"), 1);
		zephir_array_update_string(&_4, SL("model"), &modelName, PH_COPY | PH_SEPARATE);
		zephir_array_update_string(&_4, SL("column"), &sqlColumnAlias, PH_COPY | PH_SEPARATE);
		zephir_array_update_string(&_4, SL("balias"), &preparedAlias, PH_COPY | PH_SEPARATE);
		zephir_array_append(&sqlColumns, _4, PH_SEPARATE);
		RETURN_CCTOR(sqlColumns);
	}
	if (ZEPHIR_IS_LONG(columnType, 354)) {
		ZEPHIR_INIT_VAR(sqlColumn);
		array_init(sqlColumn);
		add_assoc_stringl_ex(sqlColumn, SS("type"), SL("scalar"), 1);
		zephir_array_fetch_string(&columnData, column, SL("column"), PH_NOISY | PH_READONLY TSRMLS_CC);
		ZEPHIR_INIT_VAR(sqlExprColumn);
		zephir_call_method_p1(sqlExprColumn, this_ptr, "_getexpression", columnData);
		if (zephir_array_isset_string_fetch(&balias, sqlExprColumn, SS("balias"), 1 TSRMLS_CC)) {
			zephir_array_update_string(&sqlColumn, SL("balias"), &balias, PH_COPY | PH_SEPARATE);
			zephir_array_update_string(&sqlColumn, SL("sqlAlias"), &balias, PH_COPY | PH_SEPARATE);
		}
		zephir_array_update_string(&sqlColumn, SL("column"), &sqlExprColumn, PH_COPY | PH_SEPARATE);
		zephir_array_append(&sqlColumns, sqlColumn, PH_SEPARATE);
		RETURN_CCTOR(sqlColumns);
	}
	ZEPHIR_INIT_NVAR(_4);
	object_init_ex(_4, phalcon_mvc_model_exception_ce);
	ZEPHIR_INIT_LNVAR(_5);
	ZEPHIR_CONCAT_SV(_5, "Unknown type of column ", columnType);
	zephir_call_method_p1_noret(_4, "__construct", _5);
	zephir_throw_exception(_4 TSRMLS_CC);
	ZEPHIR_MM_RESTORE();
	return;

}

/**
 * Resolves a table in a SELECT statement checking if the model exists
 *
 * @param Phalcon\Mvc\Model\ManagerInterface manager
 * @param array qualifiedName
 * @return string
 */
PHP_METHOD(Phalcon_Mvc_Model_Query, _getTable) {

	zval *manager, *qualifiedName, *modelName, *model, *source, *schema;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 0, &manager, &qualifiedName);



	if (zephir_array_isset_string_fetch(&modelName, qualifiedName, SS("name"), 1 TSRMLS_CC)) {
		ZEPHIR_INIT_VAR(model);
		zephir_call_method_p1(model, manager, "load", modelName);
		ZEPHIR_INIT_VAR(source);
		zephir_call_method(source, model, "getsource");
		ZEPHIR_INIT_VAR(schema);
		zephir_call_method(schema, model, "getschema");
		if (zephir_is_true(schema)) {
			array_init(return_value);
			zephir_array_fast_append(return_value, schema);
			zephir_array_fast_append(return_value, source);
			RETURN_MM();
		}
		RETURN_CCTOR(source);
	}
	ZEPHIR_THROW_EXCEPTION_STR(phalcon_mvc_model_exception_ce, "Corrupted SELECT AST");
	return;

}

/**
 * Resolves a JOIN clause checking if the associated models exist
 *
 * @param Phalcon\Mvc\Model\ManagerInterface manager
 * @param array join
 * @return array
 */
PHP_METHOD(Phalcon_Mvc_Model_Query, _getJoin) {

	zval *manager, *join, *qualified, *modelName, *source, *model, *schema, *_0;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 0, &manager, &join);



	if (zephir_array_isset_string_fetch(&qualified, join, SS("qualified"), 1 TSRMLS_CC)) {
		zephir_array_fetch_string(&_0, qualified, SL("type"), PH_NOISY | PH_READONLY TSRMLS_CC);
		if (ZEPHIR_IS_LONG(_0, 355)) {
			zephir_array_fetch_string(&modelName, qualified, SL("name"), PH_NOISY | PH_READONLY TSRMLS_CC);
			ZEPHIR_INIT_VAR(model);
			zephir_call_method_p1(model, manager, "load", modelName);
			ZEPHIR_INIT_VAR(source);
			zephir_call_method(source, model, "getsource");
			ZEPHIR_INIT_VAR(schema);
			zephir_call_method(schema, model, "getschema");
			array_init(return_value);
			zephir_array_update_string(&return_value, SL("schema"), &schema, PH_COPY | PH_SEPARATE);
			zephir_array_update_string(&return_value, SL("source"), &source, PH_COPY | PH_SEPARATE);
			zephir_array_update_string(&return_value, SL("modelName"), &modelName, PH_COPY | PH_SEPARATE);
			zephir_array_update_string(&return_value, SL("model"), &model, PH_COPY | PH_SEPARATE);
			RETURN_MM();
		}
	}
	ZEPHIR_THROW_EXCEPTION_STR(phalcon_mvc_model_exception_ce, "Corrupted SELECT AST");
	return;

}

/**
 * Resolves a JOIN type
 *
 * @param array join
 * @return string
 */
PHP_METHOD(Phalcon_Mvc_Model_Query, _getJoinType) {

	zval *join, *type, *_0, *_1, *_2;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &join);



	if (!(zephir_array_isset_string_fetch(&type, join, SS("type"), 1 TSRMLS_CC))) {
		ZEPHIR_THROW_EXCEPTION_STR(phalcon_mvc_model_exception_ce, "Corrupted SELECT AST");
		return;
	}
	do {
		if (ZEPHIR_IS_LONG(type, 360)) {
			RETURN_MM_STRING("INNER", 1);
		}
		if (ZEPHIR_IS_LONG(type, 361)) {
			RETURN_MM_STRING("LEFT", 1);
		}
		if (ZEPHIR_IS_LONG(type, 362)) {
			RETURN_MM_STRING("RIGHT", 1);
		}
		if (ZEPHIR_IS_LONG(type, 363)) {
			RETURN_MM_STRING("CROSS", 1);
		}
		if (ZEPHIR_IS_LONG(type, 364)) {
			RETURN_MM_STRING("FULL OUTER", 1);
		}
	} while(0);

	ZEPHIR_INIT_VAR(_0);
	object_init_ex(_0, phalcon_mvc_model_exception_ce);
	_1 = zephir_fetch_nproperty_this(this_ptr, SL("_phql"), PH_NOISY_CC);
	ZEPHIR_INIT_VAR(_2);
	ZEPHIR_CONCAT_SVSV(_2, "Unknown join type ", type, ", when preparing: ", _1);
	zephir_call_method_p1_noret(_0, "__construct", _2);
	zephir_throw_exception(_0 TSRMLS_CC);
	ZEPHIR_MM_RESTORE();
	return;

}

/**
 * Resolves joins involving has-one/belongs-to/has-many relations
 *
 * @param string joinType
 * @param string joinSource
 * @param string modelAlias
 * @param string joinAlias
 * @param Phalcon\Mvc\Model\RelationInterface relation
 * @return array
 */
PHP_METHOD(Phalcon_Mvc_Model_Query, _getSingleJoin) {

	zend_function *_8 = NULL, *_9 = NULL, *_11 = NULL;
	HashTable *_4;
	HashPosition _3;
	zval *joinType_param = NULL, *joinSource, *modelAlias, *joinAlias, *relation, *fields, *referencedFields, *sqlJoinConditions, *sqlJoinPartialConditions, *position = NULL, *field = NULL, *referencedField, *_0 = NULL, *_1 = NULL, *_2 = NULL, **_5, *_6, *_7 = NULL, *_10 = NULL;
	zval *joinType = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 5, 0, &joinType_param, &joinSource, &modelAlias, &joinAlias, &relation);

		if (Z_TYPE_P(joinType_param) != IS_STRING) {
				zephir_throw_exception_string(spl_ce_InvalidArgumentException, SL("Parameter 'joinType' must be a string") TSRMLS_CC);
				RETURN_MM_NULL();
		}

		joinType = joinType_param;



	ZEPHIR_INIT_VAR(fields);
	zephir_call_method(fields, relation, "getfields");
	ZEPHIR_INIT_VAR(referencedFields);
	zephir_call_method(referencedFields, relation, "getreferencedfields");
	if ((Z_TYPE_P(fields) != IS_ARRAY)) {
		ZEPHIR_INIT_VAR(sqlJoinConditions);
		array_init(sqlJoinConditions);
		add_assoc_stringl_ex(sqlJoinConditions, SS("type"), SL("binary-op"), 1);
		add_assoc_stringl_ex(sqlJoinConditions, SS("op"), SL("="), 1);
		ZEPHIR_INIT_VAR(_0);
		ZEPHIR_INIT_VAR(_1);
		array_init(_1);
		add_assoc_long_ex(_1, SS("type"), 355);
		zephir_array_update_string(&_1, SL("domain"), &modelAlias, PH_COPY | PH_SEPARATE);
		zephir_array_update_string(&_1, SL("name"), &fields, PH_COPY | PH_SEPARATE);
		zephir_call_method_p1(_0, this_ptr, "_getqualified", _1);
		zephir_array_update_string(&sqlJoinConditions, SL("left"), &_0, PH_COPY | PH_SEPARATE);
		ZEPHIR_INIT_NVAR(_0);
		ZEPHIR_INIT_VAR(_2);
		array_init(_2);
		add_assoc_stringl_ex(_2, SS("type"), SL("qualified"), 1);
		zephir_array_update_string(&_2, SL("domain"), &joinAlias, PH_COPY | PH_SEPARATE);
		zephir_array_update_string(&_2, SL("name"), &referencedFields, PH_COPY | PH_SEPARATE);
		zephir_call_method_p1(_0, this_ptr, "_getqualified", _2);
		zephir_array_update_string(&sqlJoinConditions, SL("right"), &_0, PH_COPY | PH_SEPARATE);
	} else {
		ZEPHIR_INIT_VAR(sqlJoinPartialConditions);
		array_init(sqlJoinPartialConditions);
		zephir_is_iterable(fields, &_4, &_3, 0, 0);
		for (
			; zend_hash_get_current_data_ex(_4, (void**) &_5, &_3) == SUCCESS
			; zend_hash_move_forward_ex(_4, &_3)
		) {
			ZEPHIR_GET_HMKEY(position, _4, _3);
			ZEPHIR_GET_HVALUE(field, _5);
			if (!(zephir_array_isset_fetch(&referencedField, referencedFields, position, 1 TSRMLS_CC))) {
				ZEPHIR_INIT_NVAR(_0);
				object_init_ex(_0, phalcon_mvc_model_exception_ce);
				_6 = zephir_fetch_nproperty_this(this_ptr, SL("_phql"), PH_NOISY_CC);
				ZEPHIR_INIT_LNVAR(_7);
				ZEPHIR_CONCAT_SVSVSV(_7, "The number of fields must be equal to the number of referenced fields in join ", modelAlias, "-", joinAlias, ", when preparing: ", _6);
				zephir_call_method_p1_cache_noret(_0, "__construct", &_8, _7);
				zephir_throw_exception(_0 TSRMLS_CC);
				ZEPHIR_MM_RESTORE();
				return;
			}
			ZEPHIR_INIT_NVAR(_0);
			array_init(_0);
			add_assoc_stringl_ex(_0, SS("type"), SL("binary-op"), 1);
			add_assoc_stringl_ex(_0, SS("op"), SL("="), 1);
			ZEPHIR_INIT_NVAR(_1);
			ZEPHIR_INIT_NVAR(_2);
			array_init(_2);
			add_assoc_long_ex(_2, SS("type"), 355);
			zephir_array_update_string(&_2, SL("domain"), &modelAlias, PH_COPY | PH_SEPARATE);
			zephir_array_update_string(&_2, SL("name"), &field, PH_COPY | PH_SEPARATE);
			zephir_call_method_p1_cache(_1, this_ptr, "_getqualified", &_9, _2);
			zephir_array_update_string(&_0, SL("left"), &_1, PH_COPY | PH_SEPARATE);
			ZEPHIR_INIT_NVAR(_1);
			ZEPHIR_INIT_NVAR(_10);
			array_init(_10);
			add_assoc_stringl_ex(_10, SS("type"), SL("qualified"), 1);
			zephir_array_update_string(&_10, SL("domain"), &joinAlias, PH_COPY | PH_SEPARATE);
			zephir_array_update_string(&_10, SL("name"), &referencedField, PH_COPY | PH_SEPARATE);
			zephir_call_method_p1_cache(_1, this_ptr, "_getqualified", &_11, _10);
			zephir_array_update_string(&_0, SL("right"), &_1, PH_COPY | PH_SEPARATE);
			zephir_array_append(&sqlJoinPartialConditions, _0, PH_SEPARATE);
		}
	}
	array_init(return_value);
	zephir_array_update_string(&return_value, SL("type"), &joinType, PH_COPY | PH_SEPARATE);
	zephir_array_update_string(&return_value, SL("source"), &joinSource, PH_COPY | PH_SEPARATE);
	zephir_array_update_string(&return_value, SL("conditions"), &sqlJoinConditions, PH_COPY | PH_SEPARATE);
	RETURN_MM();

}

/**
 * Resolves joins involving many-to-many relations
 *
 * @param string joinType
 * @param string joinSource
 * @param string modelAlias
 * @param string joinAlias
 * @param Phalcon\Mvc\Model\RelationInterface relation
 * @return array
 */
PHP_METHOD(Phalcon_Mvc_Model_Query, _getMultiJoin) {

	zend_function *_6 = NULL, *_8 = NULL, *_10 = NULL;
	HashTable *_1;
	HashPosition _0;
	zval *joinType, *joinSource, *modelAlias, *joinAlias, *relation, *sqlJoins, *fields, *referencedFields, *intermediateModelName, *intermediateModel, *intermediateSource, *intermediateSchema, *intermediateFields, *intermediateReferencedFields, *referencedModelName, *manager, *field = NULL, *position = NULL, *intermediateField, *sqlEqualsJoinCondition = NULL, **_2, *_3 = NULL, *_4, *_5 = NULL, *_7 = NULL, *_9 = NULL, *_11 = NULL, *_12, *_13, *_14, *_15;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 5, 0, &joinType, &joinSource, &modelAlias, &joinAlias, &relation);



	ZEPHIR_INIT_VAR(sqlJoins);
	array_init(sqlJoins);
	ZEPHIR_INIT_VAR(fields);
	zephir_call_method(fields, relation, "getfields");
	ZEPHIR_INIT_VAR(referencedFields);
	zephir_call_method(referencedFields, relation, "getreferencedfields");
	ZEPHIR_INIT_VAR(intermediateModelName);
	zephir_call_method(intermediateModelName, relation, "getintermediatemodel");
	manager = zephir_fetch_nproperty_this(this_ptr, SL("_manager"), PH_NOISY_CC);
	ZEPHIR_INIT_VAR(intermediateModel);
	zephir_call_method_p1(intermediateModel, manager, "load", intermediateModelName);
	ZEPHIR_INIT_VAR(intermediateSource);
	zephir_call_method(intermediateSource, intermediateModel, "getsource");
	ZEPHIR_INIT_VAR(intermediateSchema);
	zephir_call_method(intermediateSchema, intermediateModel, "getschema");
	zephir_update_property_array(this_ptr, SL("_sqlAliases"), intermediateModelName, intermediateSource TSRMLS_CC);
	zephir_update_property_array(this_ptr, SL("_sqlAliasesModelsInstances"), intermediateModelName, intermediateModel TSRMLS_CC);
	ZEPHIR_INIT_VAR(intermediateFields);
	zephir_call_method(intermediateFields, relation, "getintermediatefields");
	ZEPHIR_INIT_VAR(intermediateReferencedFields);
	zephir_call_method(intermediateReferencedFields, relation, "getintermediatereferencedfields");
	ZEPHIR_INIT_VAR(referencedModelName);
	zephir_call_method(referencedModelName, relation, "getreferencedmodel");
	if ((Z_TYPE_P(fields) == IS_ARRAY)) {
		zephir_is_iterable(fields, &_1, &_0, 0, 0);
		for (
			; zend_hash_get_current_data_ex(_1, (void**) &_2, &_0) == SUCCESS
			; zend_hash_move_forward_ex(_1, &_0)
		) {
			ZEPHIR_GET_HMKEY(field, _1, _0);
			ZEPHIR_GET_HVALUE(position, _2);
			if (!(zephir_array_isset(referencedFields, position))) {
				ZEPHIR_INIT_NVAR(_3);
				object_init_ex(_3, phalcon_mvc_model_exception_ce);
				_4 = zephir_fetch_nproperty_this(this_ptr, SL("_phql"), PH_NOISY_CC);
				ZEPHIR_INIT_LNVAR(_5);
				ZEPHIR_CONCAT_SVSVSV(_5, "The number of fields must be equal to the number of referenced fields in join ", modelAlias, "-", joinAlias, ", when preparing: ", _4);
				zephir_call_method_p1_cache_noret(_3, "__construct", &_6, _5);
				zephir_throw_exception(_3 TSRMLS_CC);
				ZEPHIR_MM_RESTORE();
				return;
			}
			zephir_array_fetch(&intermediateField, intermediateFields, position, PH_NOISY | PH_READONLY TSRMLS_CC);
			ZEPHIR_INIT_NVAR(sqlEqualsJoinCondition);
			array_init(sqlEqualsJoinCondition);
			add_assoc_stringl_ex(sqlEqualsJoinCondition, SS("type"), SL("binary-op"), 1);
			add_assoc_stringl_ex(sqlEqualsJoinCondition, SS("op"), SL("="), 1);
			ZEPHIR_INIT_NVAR(_3);
			ZEPHIR_INIT_NVAR(_7);
			array_init(_7);
			add_assoc_long_ex(_7, SS("type"), 355);
			zephir_array_update_string(&_7, SL("domain"), &modelAlias, PH_COPY | PH_SEPARATE);
			zephir_array_update_string(&_7, SL("name"), &field, PH_COPY | PH_SEPARATE);
			zephir_call_method_p1_cache(_3, this_ptr, "_getqualified", &_8, _7);
			zephir_array_update_string(&sqlEqualsJoinCondition, SL("left"), &_3, PH_COPY | PH_SEPARATE);
			ZEPHIR_INIT_NVAR(_3);
			ZEPHIR_INIT_NVAR(_9);
			array_init(_9);
			add_assoc_stringl_ex(_9, SS("type"), SL("qualified"), 1);
			zephir_array_update_string(&_9, SL("domain"), &joinAlias, PH_COPY | PH_SEPARATE);
			zephir_array_update_string(&_9, SL("name"), &referencedFields, PH_COPY | PH_SEPARATE);
			zephir_call_method_p1_cache(_3, this_ptr, "_getqualified", &_10, _9);
			zephir_array_update_string(&sqlEqualsJoinCondition, SL("right"), &_3, PH_COPY | PH_SEPARATE);
		}
	} else {
		ZEPHIR_INIT_BNVAR(sqlJoins);
		array_init(sqlJoins);
		ZEPHIR_INIT_NVAR(_3);
		array_init(_3);
		zephir_array_update_string(&_3, SL("type"), &joinType, PH_COPY | PH_SEPARATE);
		zephir_array_update_string(&_3, SL("source"), &intermediateSource, PH_COPY | PH_SEPARATE);
		ZEPHIR_INIT_NVAR(_7);
		array_init(_7);
		ZEPHIR_INIT_NVAR(_9);
		array_init(_9);
		add_assoc_stringl_ex(_9, SS("type"), SL("binary-op"), 1);
		add_assoc_stringl_ex(_9, SS("op"), SL("="), 1);
		ZEPHIR_INIT_VAR(_11);
		ZEPHIR_INIT_VAR(_12);
		array_init(_12);
		add_assoc_long_ex(_12, SS("type"), 355);
		zephir_array_update_string(&_12, SL("domain"), &modelAlias, PH_COPY | PH_SEPARATE);
		zephir_array_update_string(&_12, SL("name"), &fields, PH_COPY | PH_SEPARATE);
		zephir_call_method_p1(_11, this_ptr, "_getqualified", _12);
		zephir_array_update_string(&_9, SL("left"), &_11, PH_COPY | PH_SEPARATE);
		ZEPHIR_INIT_NVAR(_11);
		ZEPHIR_INIT_VAR(_13);
		array_init(_13);
		add_assoc_stringl_ex(_13, SS("type"), SL("qualified"), 1);
		zephir_array_update_string(&_13, SL("domain"), &intermediateModelName, PH_COPY | PH_SEPARATE);
		zephir_array_update_string(&_13, SL("name"), &intermediateFields, PH_COPY | PH_SEPARATE);
		zephir_call_method_p1(_11, this_ptr, "_getqualified", _13);
		zephir_array_update_string(&_9, SL("right"), &_11, PH_COPY | PH_SEPARATE);
		zephir_array_fast_append(_7, _9);
		zephir_array_update_string(&_3, SL("conditions"), &_7, PH_COPY | PH_SEPARATE);
		zephir_array_fast_append(sqlJoins, _3);
		ZEPHIR_INIT_NVAR(_3);
		array_init(_3);
		zephir_array_update_string(&_3, SL("type"), &joinType, PH_COPY | PH_SEPARATE);
		zephir_array_update_string(&_3, SL("source"), &joinSource, PH_COPY | PH_SEPARATE);
		ZEPHIR_INIT_NVAR(_7);
		array_init(_7);
		ZEPHIR_INIT_NVAR(_9);
		array_init(_9);
		add_assoc_stringl_ex(_9, SS("type"), SL("binary-op"), 1);
		add_assoc_stringl_ex(_9, SS("op"), SL("="), 1);
		ZEPHIR_INIT_NVAR(_11);
		ZEPHIR_INIT_VAR(_14);
		array_init(_14);
		add_assoc_long_ex(_14, SS("type"), 355);
		zephir_array_update_string(&_14, SL("domain"), &intermediateModelName, PH_COPY | PH_SEPARATE);
		zephir_array_update_string(&_14, SL("name"), &intermediateReferencedFields, PH_COPY | PH_SEPARATE);
		zephir_call_method_p1(_11, this_ptr, "_getqualified", _14);
		zephir_array_update_string(&_9, SL("left"), &_11, PH_COPY | PH_SEPARATE);
		ZEPHIR_INIT_NVAR(_11);
		ZEPHIR_INIT_VAR(_15);
		array_init(_15);
		add_assoc_stringl_ex(_15, SS("type"), SL("qualified"), 1);
		zephir_array_update_string(&_15, SL("domain"), &referencedModelName, PH_COPY | PH_SEPARATE);
		zephir_array_update_string(&_15, SL("name"), &referencedFields, PH_COPY | PH_SEPARATE);
		zephir_call_method_p1(_11, this_ptr, "_getqualified", _15);
		zephir_array_update_string(&_9, SL("right"), &_11, PH_COPY | PH_SEPARATE);
		zephir_array_fast_append(_7, _9);
		zephir_array_update_string(&_3, SL("conditions"), &_7, PH_COPY | PH_SEPARATE);
		zephir_array_fast_append(sqlJoins, _3);
	}
	RETURN_CCTOR(sqlJoins);

}

/**
 * Processes the JOINs in the query returning an internal representation for the database dialect
 *
 * @param array select
 * @return array
 */
PHP_METHOD(Phalcon_Mvc_Model_Query, _getJoins) {

	zend_class_entry *_6;
	zend_function *_3 = NULL, *_4 = NULL, *_9 = NULL, *_13 = NULL, *_20 = NULL, *_21 = NULL, *_23 = NULL, *_24 = NULL, *_25 = NULL, *_26 = NULL;
	HashTable *_1, *_11, *_15, *_18;
	HashPosition _0, _10, _14, _17;
	zval *select, *models, *sqlAliases, *sqlAliasesModels, *sqlModelsAliases, *sqlAliasesModelsInstances, *modelsInstances, *fromModels = NULL, *sqlJoins = NULL, *joinModels, *joinSources, *joinTypes, *joinPreCondition, *joinPrepared, *manager = NULL, *selectJoins = NULL, *joinItem = NULL, *joins, *joinData = NULL, *schema, *source = NULL, *model, *modelName, *completeSource = NULL, *joinType = NULL, *aliasExpr, *alias, *joinAliasName = NULL, *joinExpr, *fromModelName = NULL, *joinAlias = NULL, *joinModel = NULL, *joinSource, *preCondition, *modelNameAlias, *relation = NULL, *relations = NULL, *modelAlias, *sqlJoin = NULL, **_2, *_5 = NULL, *_7, *_8 = NULL, **_12, **_16, **_19, *_22 = NULL, *_27 = NULL, *_28 = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &select);



	ZEPHIR_OBS_VAR(models);
	zephir_read_property_this(&models, this_ptr, SL("_models"), PH_NOISY_CC);
	ZEPHIR_OBS_VAR(sqlAliases);
	zephir_read_property_this(&sqlAliases, this_ptr, SL("_sqlAliases"), PH_NOISY_CC);
	ZEPHIR_OBS_VAR(sqlAliasesModels);
	zephir_read_property_this(&sqlAliasesModels, this_ptr, SL("_sqlAliasesModels"), PH_NOISY_CC);
	ZEPHIR_OBS_VAR(sqlModelsAliases);
	zephir_read_property_this(&sqlModelsAliases, this_ptr, SL("_sqlModelsAliases"), PH_NOISY_CC);
	ZEPHIR_OBS_VAR(sqlAliasesModelsInstances);
	zephir_read_property_this(&sqlAliasesModelsInstances, this_ptr, SL("_sqlAliasesModelsInstances"), PH_NOISY_CC);
	ZEPHIR_OBS_VAR(modelsInstances);
	zephir_read_property_this(&modelsInstances, this_ptr, SL("_modelsInstances"), PH_NOISY_CC);
	ZEPHIR_CPY_WRT(fromModels, models);
	ZEPHIR_INIT_VAR(sqlJoins);
	array_init(sqlJoins);
	ZEPHIR_INIT_VAR(joinModels);
	array_init(joinModels);
	ZEPHIR_INIT_VAR(joinSources);
	array_init(joinSources);
	ZEPHIR_INIT_VAR(joinTypes);
	array_init(joinTypes);
	ZEPHIR_INIT_VAR(joinPreCondition);
	array_init(joinPreCondition);
	ZEPHIR_INIT_VAR(joinPrepared);
	array_init(joinPrepared);
	ZEPHIR_OBS_VAR(manager);
	zephir_read_property_this(&manager, this_ptr, SL("_manager"), PH_NOISY_CC);
	zephir_array_fetch_string(&joins, select, SL("joins"), PH_NOISY | PH_READONLY TSRMLS_CC);
	if (!(zephir_array_isset_long(joins, 0))) {
		ZEPHIR_INIT_VAR(selectJoins);
		array_init(selectJoins);
		zephir_array_fast_append(selectJoins, joins);
	} else {
		ZEPHIR_CPY_WRT(selectJoins, joins);
	}
	zephir_is_iterable(selectJoins, &_1, &_0, 0, 0);
	for (
		; zend_hash_get_current_data_ex(_1, (void**) &_2, &_0) == SUCCESS
		; zend_hash_move_forward_ex(_1, &_0)
	) {
		ZEPHIR_GET_HVALUE(joinItem, _2);
		ZEPHIR_INIT_NVAR(joinData);
		zephir_call_method_p2_cache(joinData, this_ptr, "_getjoin", &_3, manager, joinItem);
		ZEPHIR_OBS_NVAR(source);
		zephir_array_fetch_string(&source, joinData, SL("source"), PH_NOISY TSRMLS_CC);
		zephir_array_fetch_string(&schema, joinData, SL("schema"), PH_NOISY | PH_READONLY TSRMLS_CC);
		zephir_array_fetch_string(&model, joinData, SL("model"), PH_NOISY | PH_READONLY TSRMLS_CC);
		zephir_array_fetch_string(&modelName, joinData, SL("modelName"), PH_NOISY | PH_READONLY TSRMLS_CC);
		ZEPHIR_INIT_NVAR(completeSource);
		array_init(completeSource);
		zephir_array_fast_append(completeSource, source);
		zephir_array_fast_append(completeSource, schema);
		ZEPHIR_INIT_NVAR(joinType);
		zephir_call_method_p1_cache(joinType, this_ptr, "_getjointype", &_4, joinItem);
		if (zephir_array_isset_string_fetch(&aliasExpr, joinItem, SS("alias"), 1 TSRMLS_CC)) {
			zephir_array_fetch_string(&alias, aliasExpr, SL("name"), PH_NOISY | PH_READONLY TSRMLS_CC);
			if (zephir_array_isset(joinModels, alias)) {
				ZEPHIR_INIT_NVAR(_5);
				_6 = zend_fetch_class(SL("Phalcon_Mvc_Model_Exception"), ZEND_FETCH_CLASS_AUTO TSRMLS_CC);
				object_init_ex(_5, _6);
				zephir_throw_exception(_5 TSRMLS_CC);
				ZEPHIR_MM_RESTORE();
				return;
			}
			zephir_array_append(&completeSource, alias, PH_SEPARATE);
			zephir_array_update_zval(&joinTypes, alias, &joinType, PH_COPY | PH_SEPARATE);
			zephir_array_update_zval(&sqlAliases, alias, &alias, PH_COPY | PH_SEPARATE);
			zephir_array_update_zval(&joinModels, alias, &modelName, PH_COPY | PH_SEPARATE);
			zephir_array_update_zval(&sqlModelsAliases, modelName, &alias, PH_COPY | PH_SEPARATE);
			zephir_array_update_zval(&sqlAliasesModels, alias, &modelName, PH_COPY | PH_SEPARATE);
			zephir_array_update_zval(&sqlAliasesModelsInstances, alias, &model, PH_COPY | PH_SEPARATE);
			zephir_array_update_zval(&models, modelName, &alias, PH_COPY | PH_SEPARATE);
			zephir_array_update_zval(&joinSources, alias, &completeSource, PH_COPY | PH_SEPARATE);
			zephir_array_update_zval(&joinPrepared, alias, &joinItem, PH_COPY | PH_SEPARATE);
		} else {
			if (zephir_array_isset(joinModels, modelName)) {
				ZEPHIR_INIT_NVAR(_5);
				object_init_ex(_5, phalcon_mvc_model_exception_ce);
				_7 = zephir_fetch_nproperty_this(this_ptr, SL("_phql"), PH_NOISY_CC);
				ZEPHIR_INIT_LNVAR(_8);
				ZEPHIR_CONCAT_SVSV(_8, "Cannot use '", modelName, "' as join alias because it was already used, when preparing: ", _7);
				zephir_call_method_p1_cache_noret(_5, "__construct", &_9, _8);
				zephir_throw_exception(_5 TSRMLS_CC);
				ZEPHIR_MM_RESTORE();
				return;
			}
			zephir_array_update_zval(&joinTypes, modelName, &joinType, PH_COPY | PH_SEPARATE);
			zephir_array_update_zval(&sqlAliases, modelName, &source, PH_COPY | PH_SEPARATE);
			zephir_array_update_zval(&joinModels, modelName, &source, PH_COPY | PH_SEPARATE);
			zephir_array_update_zval(&sqlModelsAliases, modelName, &modelName, PH_COPY | PH_SEPARATE);
			zephir_array_update_zval(&sqlAliasesModels, modelName, &modelName, PH_COPY | PH_SEPARATE);
			zephir_array_update_zval(&sqlAliasesModelsInstances, modelName, &model, PH_COPY | PH_SEPARATE);
			zephir_array_update_zval(&models, modelName, &source, PH_COPY | PH_SEPARATE);
			zephir_array_update_zval(&joinSources, modelName, &completeSource, PH_COPY | PH_SEPARATE);
			zephir_array_update_zval(&joinPrepared, modelName, &joinItem, PH_COPY | PH_SEPARATE);
		}
		zephir_array_update_zval(&modelsInstances, modelName, &model, PH_COPY | PH_SEPARATE);
	}
	zephir_update_property_this(this_ptr, SL("_models"), models TSRMLS_CC);
	zephir_update_property_this(this_ptr, SL("_sqlAliases"), sqlAliases TSRMLS_CC);
	zephir_update_property_this(this_ptr, SL("_sqlAliasesModels"), sqlAliasesModels TSRMLS_CC);
	zephir_update_property_this(this_ptr, SL("_sqlModelsAliases"), sqlModelsAliases TSRMLS_CC);
	zephir_update_property_this(this_ptr, SL("_sqlAliasesModelsInstances"), sqlAliasesModelsInstances TSRMLS_CC);
	zephir_update_property_this(this_ptr, SL("_modelsInstances"), modelsInstances TSRMLS_CC);
	zephir_is_iterable(joinPrepared, &_11, &_10, 0, 0);
	for (
		; zend_hash_get_current_data_ex(_11, (void**) &_12, &_10) == SUCCESS
		; zend_hash_move_forward_ex(_11, &_10)
	) {
		ZEPHIR_GET_HMKEY(joinAliasName, _11, _10);
		ZEPHIR_GET_HVALUE(joinItem, _12);
		if (zephir_array_isset_string_fetch(&joinExpr, joinItem, SS("conditions"), 1 TSRMLS_CC)) {
			ZEPHIR_INIT_NVAR(_5);
			zephir_call_method_p1_cache(_5, this_ptr, "_getexpression", &_13, joinExpr);
			zephir_array_update_zval(&joinPreCondition, joinAliasName, &_5, PH_COPY | PH_SEPARATE);
		}
	}
	ZEPHIR_OBS_NVAR(manager);
	zephir_read_property_this(&manager, this_ptr, SL("_manager"), PH_NOISY_CC);
	zephir_is_iterable(fromModels, &_15, &_14, 0, 0);
	for (
		; zend_hash_get_current_data_ex(_15, (void**) &_16, &_14) == SUCCESS
		; zend_hash_move_forward_ex(_15, &_14)
	) {
		ZEPHIR_GET_HMKEY(fromModelName, _15, _14);
		ZEPHIR_GET_HVALUE(source, _16);
		zephir_is_iterable(joinModels, &_18, &_17, 0, 0);
		for (
			; zend_hash_get_current_data_ex(_18, (void**) &_19, &_17) == SUCCESS
			; zend_hash_move_forward_ex(_18, &_17)
		) {
			ZEPHIR_GET_HMKEY(joinAlias, _18, _17);
			ZEPHIR_GET_HVALUE(joinModel, _19);
			zephir_array_fetch(&joinSource, joinSources, joinAlias, PH_NOISY | PH_READONLY TSRMLS_CC);
			ZEPHIR_OBS_NVAR(joinType);
			zephir_array_fetch(&joinType, joinTypes, joinAlias, PH_NOISY TSRMLS_CC);
			if (!(zephir_array_isset_fetch(&preCondition, joinPreCondition, joinAlias, 1 TSRMLS_CC))) {
				zephir_array_fetch(&modelNameAlias, sqlAliasesModels, joinAlias, PH_NOISY | PH_READONLY TSRMLS_CC);
				ZEPHIR_INIT_NVAR(relation);
				zephir_call_method_p2_cache(relation, manager, "getrelationbyalias", &_20, fromModelName, modelNameAlias);
				if (ZEPHIR_IS_FALSE(relation)) {
					ZEPHIR_INIT_NVAR(relations);
					zephir_call_method_p2_cache(relations, manager, "getrelationsbetween", &_21, fromModelName, modelNameAlias);
					if ((Z_TYPE_P(relations) == IS_ARRAY)) {
						if ((zephir_fast_count_int(relations TSRMLS_CC) != 1)) {
							ZEPHIR_INIT_NVAR(_22);
							object_init_ex(_22, phalcon_mvc_model_exception_ce);
							_7 = zephir_fetch_nproperty_this(this_ptr, SL("_phql"), PH_NOISY_CC);
							ZEPHIR_INIT_LNVAR(_8);
							ZEPHIR_CONCAT_SVSVSV(_8, "There is more than one relation between models '", modelName, "' and '", joinModel, "', the join must be done using an alias, when preparing: ", _7);
							zephir_call_method_p1_cache_noret(_22, "__construct", &_23, _8);
							zephir_throw_exception(_22 TSRMLS_CC);
							ZEPHIR_MM_RESTORE();
							return;
						}
						ZEPHIR_OBS_NVAR(relation);
						zephir_array_fetch_long(&relation, relations, 0, PH_NOISY TSRMLS_CC);
					}
				}
				if ((Z_TYPE_P(relation) == IS_OBJECT)) {
					zephir_array_fetch(&modelAlias, sqlModelsAliases, fromModelName, PH_NOISY | PH_READONLY TSRMLS_CC);
					ZEPHIR_INIT_NVAR(sqlJoin);
					ZEPHIR_INIT_NVAR(_22);
					zephir_call_method_cache(_22, relation, "isthrough", &_24);
					if (!(zephir_is_true(_22))) {
						zephir_call_method_p5_cache(sqlJoin, this_ptr, "_getsinglejoin", &_25, joinType, joinSource, modelAlias, joinAlias, relation);
					} else {
						zephir_call_method_p5_cache(sqlJoin, this_ptr, "_getmultijoin", &_26, joinType, joinSource, modelAlias, joinAlias, relation);
					}
					if (zephir_array_isset_long(sqlJoin, 0)) {
						ZEPHIR_INIT_NVAR(_27);
						zephir_fast_array_merge(_27, &(sqlJoins), &(sqlJoin) TSRMLS_CC);
						ZEPHIR_CPY_WRT(sqlJoins, _27);
					} else {
						zephir_array_append(&sqlJoins, sqlJoin, PH_SEPARATE);
					}
				} else {
					ZEPHIR_INIT_NVAR(_27);
					array_init(_27);
					zephir_array_update_string(&_27, SL("type"), &joinType, PH_COPY | PH_SEPARATE);
					zephir_array_update_string(&_27, SL("source"), &joinSource, PH_COPY | PH_SEPARATE);
					ZEPHIR_INIT_NVAR(_28);
					array_init(_28);
					zephir_array_update_string(&_27, SL("conditions"), &_28, PH_COPY | PH_SEPARATE);
					zephir_array_append(&sqlJoins, _27, PH_SEPARATE);
				}
			} else {
				ZEPHIR_INIT_NVAR(_5);
				array_init(_5);
				zephir_array_update_string(&_5, SL("type"), &joinType, PH_COPY | PH_SEPARATE);
				zephir_array_update_string(&_5, SL("source"), &joinSource, PH_COPY | PH_SEPARATE);
				ZEPHIR_INIT_NVAR(_28);
				array_init(_28);
				zephir_array_fast_append(_28, preCondition);
				zephir_array_update_string(&_5, SL("conditions"), &_28, PH_COPY | PH_SEPARATE);
				zephir_array_append(&sqlJoins, _5, PH_SEPARATE);
			}
		}
	}
	RETURN_CCTOR(sqlJoins);

}

/**
 * Returns a processed order clause for a SELECT statement
 *
 * @param array $order
 * @return string
 */
PHP_METHOD(Phalcon_Mvc_Model_Query, _getOrderClause) {

	zend_function *_4 = NULL;
	HashTable *_1;
	HashPosition _0;
	zval *order, *orderColumns = NULL, *orderParts, *orderItem = NULL, *orderPartExpr = NULL, *orderSort, *orderPartSort = NULL, **_2, *_3, *_5 = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &order);



	if (!(zephir_array_isset_long(order, 0))) {
		ZEPHIR_INIT_VAR(orderColumns);
		array_init(orderColumns);
		zephir_array_fast_append(orderColumns, order);
	} else {
		ZEPHIR_CPY_WRT(orderColumns, order);
	}
	ZEPHIR_INIT_VAR(orderParts);
	array_init(orderParts);
	zephir_is_iterable(orderColumns, &_1, &_0, 0, 0);
	for (
		; zend_hash_get_current_data_ex(_1, (void**) &_2, &_0) == SUCCESS
		; zend_hash_move_forward_ex(_1, &_0)
	) {
		ZEPHIR_GET_HVALUE(orderItem, _2);
		zephir_array_fetch_string(&_3, orderItem, SL("column"), PH_NOISY | PH_READONLY TSRMLS_CC);
		ZEPHIR_INIT_NVAR(orderPartExpr);
		zephir_call_method_p1_cache(orderPartExpr, this_ptr, "_getexpression", &_4, _3);
		if (zephir_array_isset_string_fetch(&orderSort, orderItem, SS("sort"), 1 TSRMLS_CC)) {
			ZEPHIR_INIT_NVAR(orderPartSort);
			if (ZEPHIR_IS_LONG(orderSort, 327)) {
				array_init(orderPartSort);
				zephir_array_fast_append(orderPartSort, orderPartExpr);
				ZEPHIR_INIT_NVAR(_5);
				ZVAL_STRING(_5, "ASC", 1);
				zephir_array_fast_append(orderPartSort, _5);
			} else {
				array_init(orderPartSort);
				zephir_array_fast_append(orderPartSort, orderPartExpr);
				ZEPHIR_INIT_NVAR(_5);
				ZVAL_STRING(_5, "DESC", 1);
				zephir_array_fast_append(orderPartSort, _5);
			}
		} else {
			ZEPHIR_INIT_NVAR(orderPartSort);
			array_init(orderPartSort);
			zephir_array_fast_append(orderPartSort, orderPartExpr);
		}
		zephir_array_append(&orderParts, orderPartSort, PH_SEPARATE);
	}
	RETURN_CCTOR(orderParts);

}

/**
 * Sets the cache parameters of the query
 *
 * @param array cacheOptions
 * @return Phalcon\Mvc\Model\Query
 */
PHP_METHOD(Phalcon_Mvc_Model_Query, cache) {

	zval *cacheOptions;

	zephir_fetch_params(0, 1, 0, &cacheOptions);



	zephir_update_property_this(this_ptr, SL("_cacheOptions"), cacheOptions TSRMLS_CC);
	RETURN_THISW();

}

/**
 * Returns the current cache options
 *
 * @param array
 */
PHP_METHOD(Phalcon_Mvc_Model_Query, getCacheOptions) {


	RETURN_MEMBER(this_ptr, "_cacheOptions");

}

/**
 * Executes a parsed PHQL statement
 *
 * @param array bindParams
 * @param array bindTypes
 * @return mixed
 */
PHP_METHOD(Phalcon_Mvc_Model_Query, execute) {

	zval *bindParams = NULL, *bindTypes = NULL;

	zephir_fetch_params(0, 0, 2, &bindParams, &bindTypes);

	if (!bindParams) {
		bindParams = ZEPHIR_GLOBAL(global_null);
	}
	if (!bindTypes) {
		bindTypes = ZEPHIR_GLOBAL(global_null);
	}



}
