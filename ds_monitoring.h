#ifndef __DS_MONITORING_H
#define __DS_MONITORING_H
/* 
 * Data Structure Monitoring
 *
 * Records how many times did some variable or index in data structure 
 * has been visited.
 * 
 * Author: Seokjoo Cho, <lnxlht4j@gmail.com> 
 */

#include <linux/xarray.h>

#define __CONFIG_DM_DEBUG 

/**
 * struct ds_monitoring_operations - Collection of pointers to functions for 
 *                                   manipulating and managinag elements.
 * @get_idx:    (Needed)
 * 	@elem:  Pointer to any data structure that you want to watch.
 * 	Return: Value of index of which element should become.
 * @get_name:   (Optional)
 * 	@elem:  Pointer to any data structure that you want to watch.
 *	Return: Value of name with which element should be called.
 * @print_elem: (Needed)
 * 	@index: Key value for each element.
 * 	@name:  Representative name for this element.
 * 	@count: How many times has element been searched.
 * 	@percentage: Proportion of this element @count in the dm->total_counts.
 * */
struct ds_monitoring_operations {
	unsigned long (*get_index)(void *elem);
	const char * (*get_name)(void *elem);
	void (*print_elem)(unsigned long index, const char *name, 
			unsigned long long count, int percentage);
};

/**
 * struct ds_monitoring - Main container for the XArray of elements.
 * @elements:     Pointer to the XArray which contains pointers to each 
 *                ds_monitoring_elem.
 * @total_counts: Counter for how many times have xarray been called.
 * @dm_ops:       Member functions for manipulating and managing elements.
 */
struct ds_monitoring {
	struct xarray *elements;
	unsigned long long total_counts;
	const struct ds_monitoring_operations *dm_ops;
};

/**
 * struct ds_monitoring_elem - Element of XArray.
 * @key:   Index of the element itself in XArray.
 * @name:  Representative name for this element, will be used when printing elements.
 * @count: Counter for how many times have this element been searched from XArray.
 */
struct ds_monitoring_elem {
	unsigned long key;
	char *name;
	unsigned long long count;
};

/**
 * DEFINE_DS_MONITORING_OPS() - Defines ds_monitoring_operations and map each 
 *                              function pointers with real function addresses.
 * @name:        A string of your ds_monitoring name.
 * @get_idx_fn:  A function address which prepares key for a ds_monitoring_elem.
 * @get_name_fn: A function address which determines name of each ds_monitoring_elem.
 * @print_fn:    A function address which prints out for each element.
 */
#define DEFINE_DS_MONITORING_OPS(name, get_idx_fn, get_name_fn, print_fn)	\
	static const struct ds_monitoring_operations name##_dm_ops = {		\
		.get_index = get_idx_fn,					\
		.get_name = get_name_fn,					\
		.print_elem = print_fn,						\
	}

/**
 * DS_MONITORING_INIT() - Macro for Initializing ds_monitoring.
 * @xarray:  XArray to map with elements in ds_monitoring.
 * @_dm_ops: ds_monitoring_operations to map with dm->dm_ops in ds_monitoring.
 */
#define DS_MONITORING_INIT(xarray, _dm_ops) {	\
	.elements = &xarray,			\
	.total_counts = 0,			\
	.dm_ops = &_dm_ops,			\
}

/**
 * DEFINE_DS_MONITORING() - Define a Data Structure Monitoring.
 * @name:        A string that names your Data Structure Monitoring.
 * @get_idx_fn:  A function address which prepares key for a ds_monitoring_elem.
 * @get_name_fn: A function address which determines name of each 
 *               ds_monitoring_elem (pass NULL if not used).
 * @print_fn:    A function address which prints out for each element.
 *
 * This is intended for file scope definitions of Data Structure Monitoring. 
 * It declares and initialises an empty Data Structure Monitoring structure 
 * with the chosen name. It does the initialisation at compiletime instead of 
 * runtime.
 */
#define DEFINE_DS_MONITORING(name, get_idx_fn, get_name_fn, print_fn)		\
	DEFINE_XARRAY(name##_xarray);						\
	DEFINE_DS_MONITORING_OPS(name, get_idx_fn, get_name_fn, print_fn);	\
	const char *dm_name = #name;						\
	struct ds_monitoring name = DS_MONITORING_INIT(name##_xarray, name##_dm_ops);

/**
 * DECLARE_DS_MONITORING() - Declares ds_monitoring which was previously defined 
 *                           in another file.
 * @name: Name of your Data Structure Monitoring.
 */
#define DECLARE_DS_MONITORING(name)	\
	extern struct ds_monitoring name;

void find_ds_monitoring(struct ds_monitoring *dm, void *elem);
void print_ds_monitoring(struct ds_monitoring* dm);
void delete_ds_monitoring(struct ds_monitoring *dm);

#endif  /* __DS_MONITORING_H  */
