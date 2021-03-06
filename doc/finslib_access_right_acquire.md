# Libfins API Reference

### `finslib_access_right_acquire( sys, nodedata );`

### Parameters

| Parameter | Type | Description |
| :--- | :--- | :--- |
|**`sys`**|`struct fins_sys_tp *`|A pointer to a structure with the FINS context|
|**`nodedata`**|`struct fins_nodedata_tp *`|Pointer to a memory location where the node information should be stored of another node when access is currently locked by that node|

### Return Value

| Type | Description |
| :--- | :--- |
|`int`|A return value from the list [`FINS_RETVAL_...`](fins_retval.md) indicating the result of the query|

### Description

### See Also

* [`FINS_RETVAL...`](fins_retval.md) &ndash; Libfins function return code list
* [`finslib_access_log_read();`](finslib_access_log_read.md)
* [`finslib_access_right_forced_acquire();`](finslib_access_right_forced_acquire.md)
* [`finslib_access_right_release();`](finslib_access_right_release.md)
* [`finslib_write_access_log_clear();`](finslib_write_access_log_clear.md)
