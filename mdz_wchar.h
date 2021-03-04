/**
 * \ingroup mdz_string library
 *
 * \author maxdz Software GmbH
 *
 * \par license
 * This file is subject to the terms and conditions defined in file 'LICENSE.txt', which is part of this source code package.
 *
 * \par description
 * mdz_wchar is dynamically-sized contiguous string, containing C wchar_t characters.
 * 
 * Capacity - how many "wide"-characters memory is reserved for.
 * Size - how many "wide"-characters are actually used in a string, excluding terminating 0.
 * Length - actual length of string in symbols, excluding terminating 0. "surrogate pairs" count as 1 symbol.

 * "reserve/AndReserve" functions allocate/reallocate memory dynamically using malloc()/realloc().
 * "attach" functionality allows attaching contiguous block of memory to string, for using string functions on it.
 *
 * UTF-16 "surrogate pairs" are supported and checked.
 * Unicode "combining characters" are not specially-distinguished and counted as distinct symbols.
 * Endianness of bytes in "wide"-character - is always endianness of platform.
 * Size of wchar_t in bytes - is compiler-dependend. At the moment, library supports only wchar_t with size of 2 or 4 bytes.
 *
 * \par info
 * See additional info on mdz_string library like version, portability, etc in mdz_string.h
 */

#ifndef MDZ_STRING_WCHAR_H
#define MDZ_STRING_WCHAR_H

#include "mdz_types.h"

#ifdef __cplusplus
extern "C" 
{
#endif

/**
 * \defgroup Init and destroy functions
 */

/**
 * Create empty "wide"-character string with Capacity == 1 (for 0-terminator), Size == 0 and Length == 0.
 * \param nEmbedSize - size of "embedded part" of string. There is no "embedded part" if 0
 * \return:
 * NULL   - if library is not initialized with mdz_string_init() call
 * NULL   - if memory allocation failed
 * NULL   - if sizeof(wchar_t) != 2 (see description in file header)
 * Result - pointer to string for use in other mdz_wchar functions
 */
struct mdz_Wchar* mdz_wchar_create(size_t nEmbedSize);

/**
 * Create empty "wide"-character string with Capacity == 1 (for 0-terminator), Size == 0 and Length == 0.
 * Memory for wchar structure starts at position pStart. Size of internal wchar structure (it is usually bigger than mdz_Wchar!) is returned in pSize.
 * \param pStart - memory start position of wchar structure
 * \param nAreaSizeBytes - size of available memory from pStart in bytes. Should be large enough for internal wchar structure
 * \param pOutSize - returned actual size of allocated internal wchar structure in bytes, may be NULL if not needed
 * \return:
 * NULL   - if library is not initialized with mdz_string_init() call
 * NULL   - if sizeof(wchar_t) != 2 and sizeof(wchar_t) != 4 (see description in file header)
 * NULL   - if pStart == NULL or pSize == NULL
 * NULL   - if size in nSize is smaller than size of internal wchar structure
 * Result - pointer to string for use in other mdz_wchar functions. Normally it equals to pStart
 */
struct mdz_Wchar* mdz_wchar_create_attached(const void* pStart, size_t nAreaSizeBytes, size_t* pOutSize);

/**
 * Destroy "wide"-character string including underlying data. After destroying, pointer to string is set to NULL.
 * If wchar is attached using mdz_wchar_createAttached(), free() will not be called.
 * If wchar data is attached using mdz_wchar_attachData(), m_pData will not be destroyed.
 * \param ppWchar - pointer to pointer to string returned by mdz_wchar_create() or mdz_wchar_create_attached()
 */
void mdz_wchar_destroy(struct mdz_Wchar** ppWchar);

/**
 * Clear m_pData of "wide"-character string with setting Size in 0.
 * \param pWchar - pointer to string returned by mdz_wchar_create()
 */
void mdz_wchar_clear(struct mdz_Wchar* pWchar);

/**
 * Attach pre-allocated data to string, assigning pData to m_pData. If attached, m_pData will not be destroyed in mdz_wchar_destroy()
 * \param pWchar - pointer to string returned by mdz_wchar_create() or mdz_wchar_create_attached()
 * \param pData - pointer to pre-allocated data to attach
 * \param nOffsetFromStart - position in pre-allocated data to attach from. Can be > 0
 * \param nCapacity - full capacity pre-allocated data in items
 * \param enAttachType - type of attachment. 0 is expected at position pData[nOffsetFromStart] if MDZ_ATTACH_ZEROSIZE. 0 is expected at position pData[nCapacity] if MDZ_ATTACH_SIZE_TERMINATOR. MDZ_ATTACH_SIZE_NO_TERMINATOR is not allowed
 * \return:
 * mdz_false - if pWchar == NULL
 * mdz_false - if pData == NULL (MDZ_ERROR_DATA), or nOffsetFromStart >= nCapacity (MDZ_ERROR_OFFSET), or invalid enAttachType (MDZ_ERROR_ATTACHTYPE)
 * mdz_false - if enAttachType is MDZ_ATTACH_ZEROSIZE or MDZ_ATTACH_SIZE_TERMINATOR but 0 is not found at expected position (MDZ_ERROR_ATTACH_TERMINATOR)
 * mdz_false - if enAttachType == MDZ_ATTACH_SIZE_TERMINATOR and pData contains invalid "wide"-characters (MDZ_ERROR_CONTENT)
 * mdz_true  - operation succeeded
 */
mdz_bool mdz_wchar_attachData(struct mdz_Wchar* pWchar, wchar_t* pData, size_t nOffsetFromStart, size_t nCapacity, enum mdz_attach_type enAttachType);

/**
 * \defgroup Reserve capacity functions
 */

/**
 * Reserve nNewCapacity bytes for string. Size and Length do not change.
 * \param pWchar - pointer to string returned by mdz_wchar_create() or mdz_wchar_create_attached()
 * \param nNewCapacity - new capacity in "wide"-characters to reserve
 * \return:
 * mdz_false - if pWchar == NULL
 * mdz_false - if memory allocation failed (MDZ_ERROR_ALLOCATION)
 * mdz_true	 - reservation succeeded, or nNewCapacity <= Capacity (MDZ_ERROR_CAPACITY)
 */
mdz_bool mdz_wchar_reserve(struct mdz_Wchar* pWchar, size_t nNewCapacity);

/**
 * Return string Capacity in "wide"-characters.
 * \param pWchar - pointer to string returned by mdz_wchar_create() or mdz_wchar_create_attached()
 * \return:
 * SIZE_MAX - if pWchar == NULL
 * Capacity - otherwise
 */
size_t mdz_wchar_capacity(const struct mdz_Wchar* pWchar);

/**
 * Return string Size in "wide"-characters.
 * \param pWchar - pointer to string returned by mdz_wchar_create() or mdz_wchar_create_attached()
 * \return:
 * SIZE_MAX - if pWchar == NULL
 * Size     - otherwise
 */
size_t mdz_wchar_size(const struct mdz_Wchar* pWchar);

/**
 * Return string Length in symbols.
 * \param pWchar - pointer to string returned by mdz_wchar_create() or mdz_wchar_create_attached()
 * \return:
 * SIZE_MAX - if pWchar == NULL
 * Length   - otherwise
 */
size_t mdz_wchar_length(const struct mdz_Wchar* pWchar);

/**
 * Return sizeof(wchar_t) of string in bytes.
 * \param pWchar - pointer to string returned by mdz_wchar_create() or mdz_wchar_create_attached()
 * \return:
 * SIZE_MAX - if pWchar == NULL
 * sizeof(wchar_t) in bytes - otherwise
 */
size_t mdz_wchar_sizeof(const struct mdz_Wchar* pWchar);

/**
 * Return string OffsetFromStart in "wide"-characters.
 * \param pWchar - pointer to string returned by mdz_wchar_create() or mdz_wchar_create_attached()
 * \return:
 * SIZE_MAX        - if pWchar == NULL
 * OffsetFromStart - otherwise
*/
size_t mdz_wchar_offsetFromStart(const struct mdz_Wchar* pWchar);

/**
 * Return if string data is attached.
 * \param pWchar - pointer to string returned by mdz_wchar_create() or mdz_wchar_create_attached()
 * \return:
 * mdz_false - if pWchar == NULL
 * mdz_false - if string data is not attached
 * mdz_true  - if string data is attached
*/
mdz_bool mdz_wchar_isAttachedData(const struct mdz_Wchar* pWchar);

/**
 * Return string "embedded part" Size in "wide" characters.
 * \param pWchar - pointer to string returned by mdz_wchar_create() or mdz_wchar_create_attached()
 * \return:
 * SIZE_MAX - if pWchar == NULL
 * Result   - "embedded part" Size otherwise
 */
size_t mdz_wchar_embedSize(const struct mdz_Wchar* pWchar);

/**
 * \defgroup Insert/remove functions
 */

/**
 * Insert nCount "wide"-characters in string. Size grows on nCount. Length grows on symbols count.
 * \param pWchar - pointer to string returned by mdz_wchar_create() or mdz_wchar_create_attached()
 * \param nLeftPos - 0-based position to insert in symbols. "surrogate pairs" count as 1 symbol. If nLeftPos == Length or -1, items are appended. nLeftPos > Length is not allowed
 * \param pwcItems - "wide"-characters to insert
 * \param nCount - number of "wide"-characters to insert
 * \param nWcharSize - size of pwcItems wchar_t character in bytes
 * \param bReserve - if mdz_true reserve capacity when there is not enough space for insertion, otherwise mdz_false
 * \param pAsyncData - pointer to shared async data for asynchronous call, or NULL if call should be synchronous
 * \return:
 * mdz_false - if pWchar == NULL
 * mdz_false - if bReserve == mdz_true and memory allocation failed (MDZ_ERROR_ALLOCATION)
 * mdz_false - if nWcharSize is not 2 or 4 (MDZ_ERROR_WCHAR_SIZE)
 * mdz_false - if bReserve == mdz_true and there is not enough capacity for inserted data, but m_pData is attached using mdz_wchar_attachData() (MDZ_ERROR_ATTACHED)
 * mdz_false - if bReserve == mdz_false and there is not enough free Capacity in the string (MDZ_ERROR_CAPACITY)
 * mdz_false - if pwcItems contain invalid "wide"-character(s) (MDZ_ERROR_CONTENT)
 * mdz_true  - if pwcItems == NULL (MDZ_ERROR_ITEMS), or nCount == 0 (MDZ_ERROR_ZEROCOUNT), or nLeftPos > Length (MDZ_ERROR_BIGLEFT). No insertion is made
 * mdz_true  - insertion succeeded
 */
mdz_bool mdz_wchar_insertWchar_async(struct mdz_Wchar* pWchar, size_t nLeftPos, const wchar_t* pwcItems, size_t nCount, size_t nWcharSize, mdz_bool bReserve, struct mdz_asyncData* pAsyncData);

/**
 * Synchronous version
 */
#define mdz_wchar_insertWchar(pWchar, nLeftPos, pwcItems, nCount, nWcharSize, bReserve) mdz_wchar_insertWchar_async(pWchar, nLeftPos, pwcItems, nCount, nWcharSize, bReserve, NULL)

/**
 * Insert "wide"-characters string pWcharSource in string. Size grows on nCount. Length grows on symbols count.
 * \param pWchar - pointer to string returned by mdz_wchar_create() or mdz_wchar_create_attached()
 * \param nLeftPos - 0-based position to insert in symbols. "surrogate pairs" count as 1 symbol. If nLeftPos == Length or -1, items are appended. nLeftPos > Length is not allowed
 * \param pWcharSource - pointer to "wide"-characters string to insert. String is returned by mdz_wchar_create() or mdz_wchar_create_attached()
 * \param bReserve - if mdz_true reserve capacity when there is not enough space for insertion, otherwise mdz_false
 * \param pAsyncData - pointer to shared async data for asynchronous call, or NULL if call should be synchronous
 * \return:
 * mdz_false - if pWchar == NULL
 * mdz_false - if bReserve == mdz_true and memory allocation failed (MDZ_ERROR_ALLOCATION)
 * mdz_false - if bReserve == mdz_true and there is not enough capacity for inserted data, but m_pData is attached using mdz_wchar_attachData() (MDZ_ERROR_ATTACHED)
 * mdz_false - if bReserve == mdz_false and there is not enough free Capacity in the string (MDZ_ERROR_CAPACITY)
 * mdz_true  - if pWcharSource == NULL (MDZ_ERROR_SOURCE), or pWcharSource.Size == 0 (MDZ_ERROR_ZEROCOUNT), or nLeftPos > Length (MDZ_ERROR_BIGLEFT). No insertion is made
 * mdz_true  - insertion succeeded
 */
mdz_bool mdz_wchar_insertWchar_string_async(struct mdz_Wchar* pWchar, size_t nLeftPos, const struct mdz_Wchar* pWcharSource, mdz_bool bReserve, struct mdz_asyncData* pAsyncData);

/**
 * Synchronous version
 */
#define mdz_wchar_insertWchar_string(pWchar, nLeftPos, pWcharSource, bReserve) mdz_wchar_insertWchar_async(pWchar, nLeftPos, pWcharSource, bReserve, NULL)

/**
 * Insert nCount ASCII/ANSI bytes in string. Characters are converted to "wide"-characters before isertion. Size grows on added "wide"-characters count. Length grows on added symbols count.
 * \param pWchar - pointer to string returned by mdz_wchar_create() or mdz_wchar_create_attached()
 * \param nLeftPos - 0-based position to insert in symbols. "surrogate pairs" count as 1 symbol. If nLeftPos == Length or -1, items are appended. nLeftPos > Length is not allowed
 * \param pcItems - bytes to insert
 * \param nCount - number of bytes to insert
 * \param bReserve - if mdz_true reserve capacity when there is not enough space for insertion, otherwise mdz_false
 * \param pAsyncData - pointer to shared async data for asynchronous call, or NULL if call should be synchronous
 * \return:
 * mdz_false - if pWchar is NULL
 * mdz_false - if bReserve == mdz_true and memory allocation failed (MDZ_ERROR_ALLOCATION)
 * mdz_false - if bReserve == mdz_true and there is not enough capacity for inserted data, but m_pData is attached using mdz_wchar_attachData() (MDZ_ERROR_ATTACHED)
 * mdz_false - if bReserve == mdz_false and there is not enough free Capacity in the string (MDZ_ERROR_CAPACITY)
 * mdz_true  - if pcItems == NULL (MDZ_ERROR_ITEMS), or nCount == 0 (MDZ_ERROR_ZEROCOUNT), or nLeftPos > Length (MDZ_ERROR_BIGLEFT). No insertion is made
 * mdz_true  - insertion succeeded
 */
mdz_bool mdz_wchar_insertAnsi_async(struct mdz_Wchar* pWchar, size_t nLeftPos, const unsigned char* pcItems, size_t nCount, mdz_bool bReserve, struct mdz_asyncData* pAsyncData);

/**
 * Synchronous version
 */
#define mdz_wchar_insertAnsi(pWchar, nLeftPos, pcItems, nCount, bReserve) mdz_wchar_insertAnsi_async(pWchar, nLeftPos, pcItems, nCount, bReserve, NULL)

/**
 * Insert ASCII/ANSI string pAnsiSource in string. Characters are converted to "wide"-characters before isertion. Size grows on added "wide"-characters count. Length grows on added symbols count.
 * \param pWchar - pointer to string returned by mdz_wchar_create() or mdz_wchar_create_attached()
 * \param nLeftPos - 0-based position to insert in symbols. "surrogate pairs" count as 1 symbol. If nLeftPos == Length or -1, items are appended. nLeftPos > Length is not allowed
 * \param pAnsiSource - pointer to ASCII/ANSI string to insert. String is returned by mdz_ansi_create() or mdz_ansi_create_attached()
 * \param bReserve - if mdz_true reserve capacity when there is not enough space for insertion, otherwise mdz_false
 * \param pAsyncData - pointer to shared async data for asynchronous call, or NULL if call should be synchronous
 * \return:
 * mdz_false - if pWchar is NULL
 * mdz_false - if bReserve == mdz_true and memory allocation failed (MDZ_ERROR_ALLOCATION)
 * mdz_false - if bReserve == mdz_true and there is not enough capacity for inserted data, but m_pData is attached using mdz_wchar_attachData() (MDZ_ERROR_ATTACHED)
 * mdz_false - if bReserve == mdz_false and there is not enough free Capacity in the string (MDZ_ERROR_CAPACITY)
 * mdz_true  - if pAnsiSource == NULL (MDZ_ERROR_SOURCE), or pAnsiSource.Size == 0 (MDZ_ERROR_ZEROCOUNT), or nLeftPos > Length (MDZ_ERROR_BIGLEFT). No insertion is made
 * mdz_true  - insertion succeeded
 */
mdz_bool mdz_wchar_insertAnsi_string_async(struct mdz_Wchar* pWchar, size_t nLeftPos, const struct mdz_Ansi* pAnsiSource, mdz_bool bReserve, struct mdz_asyncData* pAsyncData);

/**
 * Synchronous version
 */
#define mdz_wchar_insertAnsi_string(pWchar, nLeftPos, pAnsiSource, bReserve) mdz_wchar_insertAnsi_string_async(pWchar, nLeftPos, pAnsiSourcebReserve, NULL)

/**
 * Insert nCount UTF-8 characters in string. UTF-8 characters are converted to "wide"-characters before isertion. Size grows on added "wide"-characters count. Length grows on added symbols count.
 * \param pWchar - pointer to string returned by mdz_wchar_create() or mdz_wchar_create_attached()
 * \param nLeftPos - 0-based position to insert in symbols. "surrogate pairs" count as 1 symbol. If nLeftPos == Length or -1, items are appended. nLeftPos > Length is not allowed
 * \param pcItems - UTF-8 characters to insert
 * \param nCount - number of UTF-8 characters to insert in bytes
 * \param bReserve - if mdz_true reserve capacity when there is not enough space for insertion, otherwise mdz_false
 * \param pAsyncData - pointer to shared async data for asynchronous call, or NULL if call should be synchronous
 * \return:
 * mdz_false - if pWchar is NULL
 * mdz_false - if bReserve == mdz_true and memory allocation failed (MDZ_ERROR_ALLOCATION)
 * mdz_false - if bReserve == mdz_true and there is not enough capacity for inserted data, but m_pData is attached using mdz_wchar_attachData() (MDZ_ERROR_ATTACHED)
 * mdz_false - if bReserve == mdz_false and there is not enough free Capacity in the string (MDZ_ERROR_CAPACITY)
 * mdz_false - if pcItems contain invalid UTF-8 character(s) (MDZ_ERROR_CONTENT)
 * mdz_true  - if pcItems == NULL (MDZ_ERROR_ITEMS), or nCount == 0 (MDZ_ERROR_ZEROCOUNT), or nLeftPos > Length (MDZ_ERROR_BIGLEFT). No insertion is made
 * mdz_true  - insertion succeeded
 */
mdz_bool mdz_wchar_insertUtf8_async(struct mdz_Wchar* pWchar, size_t nLeftPos, const unsigned char* pcItems, size_t nCount, mdz_bool bReserve, struct mdz_asyncData* pAsyncData);

/**
 * Synchronous version
 */
#define mdz_wchar_insertUtf8(pWchar, nLeftPos, pcItems, nCount, bReserve) mdz_wchar_insertUtf8_async(pWchar, nLeftPos, pcItems, nCount, bReserve, NULL)

/**
 * Insert UTF-8 string pUtf8Source in string. UTF-8 characters are converted to "wide"-characters before isertion. Size grows on added "wide"-characters count. Length grows on added symbols count.
 * \param pWchar - pointer to string returned by mdz_wchar_create() or mdz_wchar_create_attached()
 * \param nLeftPos - 0-based position to insert in symbols. "surrogate pairs" count as 1 symbol. If nLeftPos == Length or -1, items are appended. nLeftPos > Length is not allowed
 * \param pUtf8Source - pointer to UTF-8 string to insert. String is returned by mdz_utf8_create() or mdz_utf8_create_attached()
 * \param bReserve - if mdz_true reserve capacity when there is not enough space for insertion, otherwise mdz_false
 * \param pAsyncData - pointer to shared async data for asynchronous call, or NULL if call should be synchronous
 * \return:
 * mdz_false - if pWchar is NULL
 * mdz_false - if bReserve == mdz_true and memory allocation failed (MDZ_ERROR_ALLOCATION)
 * mdz_false - if bReserve == mdz_true and there is not enough capacity for inserted data, but m_pData is attached using mdz_wchar_attachData() (MDZ_ERROR_ATTACHED)
 * mdz_false - if bReserve == mdz_false and there is not enough free Capacity in the string (MDZ_ERROR_CAPACITY)
 * mdz_true  - if pUtf8Source == NULL (MDZ_ERROR_SOURCE), or pUtf8Source.Size == 0 (MDZ_ERROR_ZEROCOUNT), or nLeftPos > Length (MDZ_ERROR_BIGLEFT). No insertion is made
 * mdz_true  - insertion succeeded
 */
mdz_bool mdz_wchar_insertUtf8_string_async(struct mdz_Wchar* pWchar, size_t nLeftPos, struct mdz_Utf8* pUtf8Source, mdz_bool bReserve, struct mdz_asyncData* pAsyncData);

/**
 * Synchronous version
 */
#define mdz_wchar_insertUtf8_string(pWchar, nLeftPos, pUtf8Source, bReserve) mdz_wchar_insertUtf8_string_async(pWchar, nLeftPos, pUtf8Source, bReserve, NULL)

/**
 * Insert nCount UTF-16 characters in string. UTF-16 characters are converted to "wide"-characters before isertion. Size grows on added "wide"-characters count. Length grows on added symbols count.
 * \param pWchar - pointer to string returned by mdz_wchar_create() or mdz_wchar_create_attached()
 * \param nLeftPos - 0-based position to insert in symbols. "surrogate pairs" count as 1 symbol. If nLeftPos == Length or -1, items are appended. nLeftPos > Length is not allowed
 * \param pItems - UTF-16 characters to insert
 * \param nCount - number of UTF-16 characters to insert
 * \param enEndianness - endianness of UTF-16 characters in pItems. Can be MDZ_ENDIAN_LITTLE for "little-endian" or MDZ_ENDIAN_BIG for "big-endian"
 * \param bReserve - if mdz_true reserve capacity when there is not enough space for insertion, otherwise mdz_false
 * \param pAsyncData - pointer to shared async data for asynchronous call, or NULL if call should be synchronous
 * \return:
 * mdz_false - if pWchar is NULL
 * mdz_false - if bReserve == mdz_true and memory allocation failed (MDZ_ERROR_ALLOCATION)
 * mdz_false - if bReserve == mdz_true and there is not enough capacity for inserted data, but m_pData is attached using mdz_wchar_attachData() (MDZ_ERROR_ATTACHED)
 * mdz_false - if bReserve == mdz_false and there is not enough free Capacity in the string (MDZ_ERROR_CAPACITY)
 * mdz_false - if pItems contain invalid UTF-16 character(s) (MDZ_ERROR_CONTENT), or invalid enEndianness (MDZ_ERROR_ENDIANNESS)
 * mdz_true  - if pItems == NULL (MDZ_ERROR_ITEMS), or nCount == 0 (MDZ_ERROR_ZEROCOUNT), or nLeftPos > Length (MDZ_ERROR_BIGLEFT). No insertion is made
 * mdz_true  - insertion succeeded
 */
mdz_bool mdz_wchar_insertUtf16_async(struct mdz_Wchar* pWchar, size_t nLeftPos, const uint16_t* pItems, size_t nCount, enum mdz_endianness enEndianness, mdz_bool bReserve, struct mdz_asyncData* pAsyncData);

/**
 * Synchronous version
 */
#define mdz_wchar_insertUtf16(pWchar, nLeftPos, pItems, nCount, enEndianness, bReserve) mdz_wchar_insertUtf16_async(pWchar, nLeftPos, pItems, nCount, enEndianness, bReserve, NULL)

/**
 * Insert UTF-16 string pUtf16Source in string. UTF-16 characters are converted to "wide"-characters before isertion. Size grows on added "wide"-characters count. Length grows on added symbols count.
 * \param pWchar - pointer to string returned by mdz_wchar_create() or mdz_wchar_create_attached()
 * \param nLeftPos - 0-based position to insert in symbols. "surrogate pairs" count as 1 symbol. If nLeftPos == Length or -1, items are appended. nLeftPos > Length is not allowed
 * \param pUtf16Source - pointer to UTF-16 string to insert. String is returned by mdz_utf16_create() or mdz_utf16_create_attached()
 * \param bReserve - if mdz_true reserve capacity when there is not enough space for insertion, otherwise mdz_false
 * \param pAsyncData - pointer to shared async data for asynchronous call, or NULL if call should be synchronous
 * \return:
 * mdz_false - if pWchar is NULL
 * mdz_false - if bReserve == mdz_true and memory allocation failed (MDZ_ERROR_ALLOCATION)
 * mdz_false - if bReserve == mdz_true and there is not enough capacity for inserted data, but m_pData is attached using mdz_wchar_attachData() (MDZ_ERROR_ATTACHED)
 * mdz_false - if bReserve == mdz_false and there is not enough free Capacity in the string (MDZ_ERROR_CAPACITY)
 * mdz_true  - if pUtf16Source == NULL (MDZ_ERROR_SOURCE), or pUtf16Source.Size == 0 (MDZ_ERROR_ZEROCOUNT), or nLeftPos > Length (MDZ_ERROR_BIGLEFT). No insertion is made
 * mdz_true  - insertion succeeded
 */
mdz_bool mdz_wchar_insertUtf16_string_async(struct mdz_Wchar* pWchar, size_t nLeftPos, struct mdz_Utf16* pUtf16Source, mdz_bool bReserve, struct mdz_asyncData* pAsyncData);

/**
 * Synchronous version
 */
#define mdz_wchar_insertUtf16_string(pWchar, nLeftPos, pUtf16Source, bReserve) mdz_wchar_insertUtf16_string_async(pWchar, nLeftPos, pUtf16Source, bReserve, NULL)

/**
 * Insert nCount UTF-32 characters in string. UTF-32 characters are converted to "wide"-characters before isertion. Size grows on added "wide"-characters count. Length grows on added symbols count.
 * \param pWchar - pointer to string returned by mdz_wchar_create() or mdz_wchar_create_attached()
 * \param nLeftPos - 0-based position to insert in symbols. "surrogate pairs" count as 1 symbol. If nLeftPos == Length or -1, items are appended. nLeftPos > Length is not allowed
 * \param pItems - UTF-32 characters to insert
 * \param nCount - number of UTF-32 characters to insert
 * \param enEndianness - endianness of UTF-32 characters in pItems. Can be MDZ_ENDIAN_LITTLE for "little-endian" or MDZ_ENDIAN_BIG for "big-endian"
 * \param bReserve - if mdz_true reserve capacity when there is not enough space for insertion, otherwise mdz_false
 * \param pAsyncData - pointer to shared async data for asynchronous call, or NULL if call should be synchronous
 * \return:
 * mdz_false - if pWchar is NULL
 * mdz_false - if bReserve == mdz_true and memory allocation failed (MDZ_ERROR_ALLOCATION)
 * mdz_false - if bReserve == mdz_true and there is not enough capacity for inserted data, but m_pData is attached using mdz_wchar_attachData() (MDZ_ERROR_ATTACHED)
 * mdz_false - if bReserve == mdz_false and there is not enough free Capacity in the string (MDZ_ERROR_CAPACITY)
 * mdz_false - if pItems contain invalid UTF-32 character(s) (MDZ_ERROR_CONTENT), or invalid enEndianness (MDZ_ERROR_ENDIANNESS)
 * mdz_true  - if pItems == NULL (MDZ_ERROR_ITEMS), or nCount == 0 (MDZ_ERROR_ZEROCOUNT), or nLeftPos > Length (MDZ_ERROR_BIGLEFT). No insertion is made
 * mdz_true  - insertion succeeded
 */
mdz_bool mdz_wchar_insertUtf32_async(struct mdz_Wchar* pWchar, size_t nLeftPos, const uint32_t* pItems, size_t nCount, enum mdz_endianness enEndianness, mdz_bool bReserve, struct mdz_asyncData* pAsyncData);

/**
 * Synchronous version
 */
#define mdz_wchar_insertUtf32(pWchar, nLeftPos, pItems, nCount, enEndianness, bReserve) mdz_wchar_insertUtf32_async(pWchar, nLeftPos, pItems, nCount, enEndianness, bReserve, NULL)

/**
 * Insert UTF-32 string pUtf32Source in string. UTF-32 characters are converted to "wide"-characters before isertion. Size grows on added "wide"-characters count. Length grows on added symbols count.
 * \param pWchar - pointer to string returned by mdz_wchar_create() or mdz_wchar_create_attached()
 * \param nLeftPos - 0-based position to insert in symbols. "surrogate pairs" count as 1 symbol. If nLeftPos == Length or -1, items are appended. nLeftPos > Length is not allowed
 * \param pUtf32Source - pointer to UTF-32 string to insert. String is returned by mdz_utf32_create() or mdz_utf32_create_attached()
 * \param bReserve - if mdz_true reserve capacity when there is not enough space for insertion, otherwise mdz_false
 * \param pAsyncData - pointer to shared async data for asynchronous call, or NULL if call should be synchronous
 * \return:
 * mdz_false - if pWchar is NULL
 * mdz_false - if bReserve == mdz_true and memory allocation failed (MDZ_ERROR_ALLOCATION)
 * mdz_false - if bReserve == mdz_true and there is not enough capacity for inserted data, but m_pData is attached using mdz_wchar_attachData() (MDZ_ERROR_ATTACHED)
 * mdz_false - if bReserve == mdz_false and there is not enough free Capacity in the string (MDZ_ERROR_CAPACITY)
 * mdz_true  - if pUtf32Source == NULL (MDZ_ERROR_SOURCE), or pUtf32Source.Size == 0 (MDZ_ERROR_ZEROCOUNT), or nLeftPos > Length (MDZ_ERROR_BIGLEFT). No insertion is made
 * mdz_true  - insertion succeeded
 */
mdz_bool mdz_wchar_insertUtf32_string_async(struct mdz_Wchar* pWchar, size_t nLeftPos, struct mdz_Utf32* pUtf32Source, mdz_bool bReserve, struct mdz_asyncData* pAsyncData);

/**
 * Synchronous version
 */
#define mdz_wchar_insertUtf32_string(pWchar, nLeftPos, pUtf32Source, bReserve) mdz_wchar_insertUtf32_string_async(pWchar, nLeftPos, pUtf32Source, bReserve, NULL)

#ifdef __cplusplus
}
#endif

#endif
