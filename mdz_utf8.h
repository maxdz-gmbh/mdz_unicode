/**
 * \ingroup mdz_string library
 *
 * \author maxdz Software GmbH
 *
 * \par license
 * This file is subject to the terms and conditions defined in file 'LICENSE.txt', which is part of this source code package.
 *
 * \par description
 * mdz_utf8 is dynamically-sized contiguous string, containing UTF-8 characters
 *
 * If only ASCII/ANSI characters should be stored/processed - use mdz_ansi instead. It is much more speedy.
 *
 * Capacity - how many bytes of memory is reserved.
 * Size - how many bytes are actually used in a string, excluding terminating 0.
 * Length - actual length of string in UTF-8 symbols, excluding terminating 0.
 *
 * "reserve/AndReserve" functions allocate/reallocate memory dynamically using malloc()/realloc().
 * "attach" functionality allows attaching contiguous block of memory to string, for using string functions on it.
 *
 * Unicode "combining characters" are not specially-distinguished and counted as a distinct symbols.
 *
 * \par info
 * See additional info on mdz_string library like version, portability, etc in mdz_string.h
 */

#ifndef MDZ_STRING_UTF8_H
#define MDZ_STRING_UTF8_H

#include "mdz_types.h"

#include <stdint.h>

#ifdef __cplusplus
extern "C" 
{
#endif

/**
 * \defgroup Init and destroy functions
 */

/**
 * Create empty UTF-8 string with Capacity == 1 (for 0-terminator), Size == 0 and Length == 0.
 * \param nEmbedSize - size of "embedded part" of string. There is no "embedded part" if 0
 * \return:
 * NULL   - if library is not initialized with mdz_string_init() call
 * NULL   - if memory allocation failed
 * Result - pointer to string for use in other mdz_utf8 functions
 */
struct mdz_Utf8* mdz_utf8_create(size_t nEmbedSize);

/**
 * Create empty UTF-8 string with Capacity == 1 (for 0-terminator), Size == 0 and Length == 0.
 * Memory for utf8 structure starts at position pStart. Size of internal utf8 structure (it is usually bigger than mdz_Utf8!) is returned in pSize.
 * \param pStart - memory start position of utf8 structure
 * \param nAreaSizeBytes - size of available memory from pStart in bytes. Should be large enough for internal utf8 structure
 * \param pOutSize - returned actual size of placed  internal utf8 structure in bytes, may be NULL if not needed
 * \return:
 * NULL   - if library is not initialized with mdz_string_init() call
 * NULL   - if pStart == NULL or pSize == NULL
 * NULL   - if size in nSize is smaller than size of internal utf8 structure
 * Result - pointer to string for use in other mdz_utf8 functions. Normally it equals to pStart
 */
struct mdz_Utf8* mdz_utf8_create_attached(const void* pStart, size_t nAreaSizeBytes, size_t* pOutSize);

/**
 * Destroy UTF-8 string including underlying data. After destroying, pointer to string is set to NULL.
 * If utf8 is attached using mdz_utf8_createAttached(), free() will not be called.
 * If utf8 data is attached using mdz_utf8_attachData(), m_pData will not be destroyed.
 * \param ppUtf8 - pointer to pointer to string returned by mdz_utf8_create() or mdz_utf8_create_attached()
 */
void mdz_utf8_destroy(struct mdz_Utf8** ppUtf8);

/**
 * Clear m_pData of UTF-8 string with setting Size in 0.
 * \param pUtf8 - pointer to string returned by mdz_utf8_create() or mdz_utf8_create_attached()
 */
void mdz_utf8_clear(struct mdz_Utf8* pUtf8);

/**
 * Attach pre-allocated data to UTF-8 string, assigning pData to m_pData. If attached, m_pData will not be destroyed in mdz_utf8_destroy()
 * \param pUtf8 - pointer to string returned by mdz_utf8_create() or mdz_utf8_create_attached()
 * \param pData - pointer to pre-allocated data to attach
 * \param nOffsetFromStart - position in pre-allocated data to attach from. Can be > 0
 * \param nCapacity - full capacity pre-allocated data in items
 * \param enAttachType - type of attachment. Only MDZ_ATTACH_ZEROSIZE and MDZ_ATTACH_SIZE_TERMINATOR are allowed
 * \return:
 * mdz_false - if pUtf8 == NULL
 * mdz_false - if test-license is used and nCapacity > 1200 (MDZ_ERROR_TEST_CAPACITY)
 * mdz_false - if pData == NULL (MDZ_ERROR_DATA), or nOffsetFromStart >= nCapacity (MDZ_ERROR_OFFSET), or invalid enAttachType (MDZ_ERROR_ATTACHTYPE)
 * mdz_false - if enAttachType == MDZ_ATTACH_SIZE_TERMINATOR and pData contains invalid UTF-8 characters (MDZ_ERROR_CONTENT)
 * mdz_true  - operation succeeded
 */
mdz_bool mdz_utf8_attachData(struct mdz_Utf8* pUtf8, unsigned char* pData, size_t nOffsetFromStart, size_t nCapacity, enum mdz_attach_type enAttachType);

/*mdz_bool mdz_utf8_detachData(struct mdz_Utf8* pUtf8);*/

/**
 * \defgroup Reserve capacity functions
 */

/**
 * Reserve nNewCapacity bytes for UTF-8 string. String Size and Length do not change.
 * \param pUtf8 - pointer to string returned by mdz_utf8_create() or mdz_utf8_create_attached()
 * \param nNewCapacity - new capacity in bytes to reserve
 * \return:
 * mdz_false - if pUtf8 == NULL 
 * mdz_false - if memory allocation failed (MDZ_ERROR_ALLOCATION)
 * mdz_false - if test-license is used and nNewCapacity > 1200 (MDZ_ERROR_TEST_CAPACITY)
 * mdz_true	 - reservation succeeded, or nNewCapacity <= Capacity (MDZ_ERROR_CAPACITY)
 */
mdz_bool mdz_utf8_reserve(struct mdz_Utf8* pUtf8, size_t nNewCapacity);

/**
 * Return string Capacity in bytes.
 * \param pUtf8 - pointer to string returned by mdz_utf8_create()
 * \return:
 * SIZE_MAX - if pUtf8 == NULL
 * Capacity - otherwise
 */
size_t mdz_utf8_capacity(const struct mdz_Utf8* pUtf8);

/**
 * Return string Size in bytes.
 * \param pUtf8 - pointer to string returned by mdz_utf8_create() or mdz_utf8_create_attached()
 * \return:
 * SIZE_MAX - if pUtf8 == NULL
 * Size     - otherwise
 */
size_t mdz_utf8_size(const struct mdz_Utf8* pUtf8);

/**
 * Return string Length in symbols.
 * \param pUtf8 - pointer to string returned by mdz_utf8_create() or mdz_utf8_create_attached()
 * \return:
 * SIZE_MAX - if pUtf8 == NULL
 * Length   - otherwise
 */
size_t mdz_utf8_length(const struct mdz_Utf8* pUtf8);

/**
 * Return string OffsetFromStart in bytes.
 * \param pUtf8 - pointer to string returned by mdz_utf8_create() or mdz_utf8_create_attached()
 * \return:
 * SIZE_MAX        - if pUtf8 == NULL
 * OffsetFromStart - otherwise
*/
size_t mdz_utf8_offsetFromStart(const struct mdz_Utf8* pUtf8);

/**
 * Return if string data is attached.
 * \param pUtf8 - pointer to string returned by mdz_utf8_create() or mdz_utf8_create_attached()
 * \return:
 * mdz_false - if pUtf8 == NULL
 * mdz_false - if string data is not attached
 * mdz_true  - if string data is attached
*/
mdz_bool mdz_utf8_isAttachedData(const struct mdz_Utf8* pUtf8);

/**
 * Return string "embedded part" Size in bytes.
 * \param pUtf8 - pointer to string returned by mdz_utf8_create() or mdz_utf8_create_attached()
 * \return:
 * SIZE_MAX - if pUtf8 == NULL
 * Result   - "embedded part" Size otherwise
 */
size_t mdz_utf8_embedSize(const struct mdz_Utf8* pUtf8);

/**
 * \defgroup Insert/remove functions
 */

/**
 * Insert nCount UTF-8 bytes in string. Size grows on nCount. Length grows on symbols count.
 * \param pUtf8 - pointer to string returned by mdz_utf8_create() or mdz_utf8_create_attached()
 * \param nLeftPos - 0-based position to insert in symbols. If nLeftPos == Length or -1, items are appended. nLeftPos > Length is not allowed
 * \param pcItems - UTF-8 bytes to insert
 * \param nCount - number of UTF-8 bytes to insert or 0 if pcItems until 0-terminator should be used
 * \param bReserve - if mdz_true reserve capacity when there is not enough space for insertion, otherwise mdz_false
 * \param pAsyncData - pointer to shared async data for asynchronous call, or NULL if call should be synchronous
 * \return:
 * mdz_false - if pUtf8 == NULL
 * mdz_false - if bReserve == mdz_true and memory allocation failed (MDZ_ERROR_ALLOCATION)
 * mdz_false - if bReserve == mdz_true and there is not enough capacity for inserted data, but m_pData is attached using mdz_utf8_attachData() (MDZ_ERROR_ATTACHED)
 * mdz_false - if bReserve == mdz_false and there is not enough free Capacity in the string (MDZ_ERROR_CAPACITY)
 * mdz_false - if pcItems contain invalid UTF-8 byte(s) (MDZ_ERROR_CONTENT)
 * mdz_true  - if pcItems == NULL (MDZ_ERROR_ITEMS), or nCount == 0 (MDZ_ERROR_ZEROCOUNT), or nLeftPos > Length (MDZ_ERROR_BIGLEFT). No insertion is made
 * mdz_true  - insertion succeeded
 */
mdz_bool mdz_utf8_insertUtf8_async(struct mdz_Utf8* pUtf8, size_t nLeftPos, const unsigned char* pcItems, size_t nCount, mdz_bool bReserve, struct mdz_asyncData* pAsyncData);

/**
 * Synchronous version
 */
#define mdz_utf8_insertUtf8(pUtf8, nLeftPos, pcItems, nCount, bReserve) mdz_utf8_insertUtf8_async(pUtf8, nLeftPos, pcItems, nCount, bReserve, NULL)

/**
 * Insert UTF-8 string pUtf8Source in string. Size grows on nCount. Length grows on symbols count.
 * This function performs significantly better than mdz_utf8_insertUtf8_async() - because there is no additional validation of inserted string.
 * \param pUtf8 - pointer to string returned by mdz_utf8_create() or mdz_utf8_create_attached()
 * \param nLeftPos - 0-based position to insert in symbols. If nLeftPos == Length or -1, items are appended. nLeftPos > Length is not allowed
 * \param pUtf8Source - pointer to UTF-8 string to insert. String is returned by mdz_utf8_create() or mdz_utf8_create_attached()
 * \param bReserve - if mdz_true reserve capacity when there is not enough space for insertion, otherwise mdz_false
 * \param pAsyncData - pointer to shared async data for asynchronous call, or NULL if call should be synchronous
 * \return:
 * mdz_false - if pUtf8 == NULL
 * mdz_false - if bReserve == mdz_true and memory allocation failed (MDZ_ERROR_ALLOCATION)
 * mdz_false - if bReserve == mdz_true and there is not enough capacity for inserted data, but m_pData is attached using mdz_utf8_attachData() (MDZ_ERROR_ATTACHED)
 * mdz_false - if bReserve == mdz_false and there is not enough free Capacity in the string (MDZ_ERROR_CAPACITY)
 * mdz_true  - if pUtf8Source == NULL (MDZ_ERROR_SOURCE), or pUtf8Source.Size == 0 (MDZ_ERROR_ZEROCOUNT), or nLeftPos > Length (MDZ_ERROR_BIGLEFT). No insertion is made
 * mdz_true  - insertion succeeded
 */
mdz_bool mdz_utf8_insertUtf8_string_async(struct mdz_Utf8* pUtf8, size_t nLeftPos, const struct mdz_Utf8* pUtf8Source, mdz_bool bReserve, struct mdz_asyncData* pAsyncData);

/**
 * Synchronous version
 */
#define mdz_utf8_insertUtf8_string(pUtf8, nLeftPos, pUtf8Source, bReserve) mdz_utf8_insertUtf8_string_async(pUtf8, nLeftPos, pUtf8Source, bReserve, NULL)

/**
 * Insert nCount ASCII/ANSI bytes in string. Characters are converted to UTF-8 characters before isertion. Size grows on added bytes. Length grows on added symbols count.
 * \param pUtf8 - pointer to string returned by mdz_utf8_create() or mdz_utf8_create_attached()
 * \param nLeftPos - 0-based position to insert in symbols. If nLeftPos == Length or -1, items are appended. nLeftPos > Length is not allowed
 * \param pcItems - bytes to insert
 * \param nCount - number of bytes to insert or 0 if pcItems until 0-terminator should be used
 * \param bReserve - if mdz_true reserve capacity when there is not enough space for insertion, otherwise mdz_false
 * \param pAsyncData - pointer to shared async data for asynchronous call, or NULL if call should be synchronous
 * \return:
 * mdz_false - if pUtf8 == NULL
 * mdz_false - if bReserve == mdz_true and memory allocation failed (MDZ_ERROR_ALLOCATION)
 * mdz_false - if bReserve == mdz_true and there is not enough capacity for inserted data, but m_pData is attached using mdz_utf8_attachData() (MDZ_ERROR_ATTACHED)
 * mdz_false - if bReserve == mdz_false and there is not enough free Capacity in the string (MDZ_ERROR_CAPACITY)
 * mdz_true  - if pcItems == NULL (MDZ_ERROR_ITEMS), or nCount == 0 (MDZ_ERROR_ZEROCOUNT), or nLeftPos > Length (MDZ_ERROR_BIGLEFT). No insertion is made
 * mdz_true  - insertion succeeded
 */
mdz_bool mdz_utf8_insertAnsi_async(struct mdz_Utf8* pUtf8, size_t nLeftPos, const unsigned char* pcItems, size_t nCount, mdz_bool bReserve, struct mdz_asyncData* pAsyncData);

/**
 * Synchronous version
 */
#define mdz_utf8_insertAnsi(pUtf8, nLeftPos, pcItems, nCount, bReserve) mdz_utf8_insertAnsi_async(pUtf8, nLeftPos, pcItems, nCount, bReserve, NULL)

/**
 * Insert ASCII/ANSI string pAnsiSource in string. Characters are converted to UTF-8 characters before isertion. Size grows on added bytes. Length grows on added symbols count.
 * \param pUtf8 - pointer to string returned by mdz_utf8_create() or mdz_utf8_create_attached()
 * \param nLeftPos - 0-based position to insert in symbols. If nLeftPos == Length or -1, items are appended. nLeftPos > Length is not allowed
 * \param pAnsiSource - pointer to ASCII/ANSI string to insert. String is returned by mdz_ansi_create() or mdz_ansi_create_attached()
 * \param bReserve - if mdz_true reserve capacity when there is not enough space for insertion, otherwise mdz_false
 * \param pAsyncData - pointer to shared async data for asynchronous call, or NULL if call should be synchronous
 * \return:
 * mdz_false - if pUtf8 == NULL
 * mdz_false - if bReserve == mdz_true and memory allocation failed (MDZ_ERROR_ALLOCATION)
 * mdz_false - if bReserve == mdz_true and there is not enough capacity for inserted data, but m_pData is attached using mdz_utf8_attachData() (MDZ_ERROR_ATTACHED)
 * mdz_false - if bReserve == mdz_false and there is not enough free Capacity in the string (MDZ_ERROR_CAPACITY)
 * mdz_true  - if pAnsiSource == NULL (MDZ_ERROR_SOURCE), or pAnsiSource.Size == 0 (MDZ_ERROR_ZEROCOUNT), or nLeftPos > Length (MDZ_ERROR_BIGLEFT). No insertion is made
 * mdz_true  - insertion succeeded
 */
mdz_bool mdz_utf8_insertAnsi_string_async(struct mdz_Utf8* pUtf8, size_t nLeftPos, const struct mdz_Ansi* pAnsiSource, mdz_bool bReserve, struct mdz_asyncData* pAsyncData);

/**
 * Synchronous version
 */
#define mdz_utf8_insertAnsi_string(pUtf8, nLeftPos, pcItems, nCount, bReserve) mdz_utf8_insertAnsi_string_async(pUtf8, nLeftPos, pcItems, nCount, bReserve, NULL)

/**
 * Insert nCount "wide"-characters in string. Characters are converted to UTF-8 characters before isertion. Size grows on added bytes. Length grows on symbols count.
 * \param pUtf8 - pointer to string returned by mdz_utf8_create() or mdz_utf8_create_attached()
 * \param nLeftPos - 0-based position to insert in symbols. "surrogate pairs" count as 1 symbol. If nLeftPos == Length or -1, items are appended. nLeftPos > Length is not allowed
 * \param pwcItems - "wide"-characters to insert
 * \param nCount - number of "wide"-characters to insert or 0 if characters until 0-terminator should be used
 * \param bReserve - if mdz_true reserve capacity when there is not enough space for insertion, otherwise mdz_false
 * \param pAsyncData - pointer to shared async data for asynchronous call, or NULL if call should be synchronous
 * \return:
 * mdz_false - if pUtf8 == NULL
 * mdz_false - if bReserve == mdz_true and memory allocation failed (MDZ_ERROR_ALLOCATION)
 * mdz_false - if bReserve == mdz_true and there is not enough capacity for inserted data, but m_pData is attached using mdz_utf8_attachData() (MDZ_ERROR_ATTACHED)
 * mdz_false - if bReserve == mdz_false and there is not enough free Capacity in the string (MDZ_ERROR_CAPACITY)
 * mdz_false - if pwcItems contain invalid "wide"-character(s) (MDZ_ERROR_CONTENT)
 * mdz_true  - if pwcItems == NULL (MDZ_ERROR_ITEMS), or nCount == 0 (MDZ_ERROR_ZEROCOUNT), or nLeftPos > Length (MDZ_ERROR_BIGLEFT). No insertion is made
 * mdz_true  - insertion succeeded
 */
mdz_bool mdz_utf8_insertWchar_async(struct mdz_Utf8* pUtf8, size_t nLeftPos, const wchar_t* pwcItems, size_t nCount, mdz_bool bReserve, struct mdz_asyncData* pAsyncData);

/**
 * Synchronous version
 */
#define mdz_utf8_insertWchar(pUtf8, nLeftPos, pwcItems, nCount, bReserve) mdz_utf8_insertWchar_async(pUtf8, nLeftPos, pwcItems, nCount, bReserve, NULL)

/**
 * Insert "wide"-characters string pWcharSource in string. Characters are converted to UTF-8 characters before isertion. Size grows on added bytes. Length grows on symbols count.
 * For 2-bytes "wide"-characters - see information concerning Unicode "surrogate pairs"/"combining characters" in description of mdz_utf8_insertUtf16_async().
 * For 4-bytes "wide"-characters - see information concerning Unicode "combining characters" in description of mdz_utf8_insertUtf32_async().
 * \param pUtf8 - pointer to string returned by mdz_utf8_create() or mdz_utf8_create_attached()
 * \param nLeftPos - 0-based position to insert in symbols. "surrogate pairs" count as 1 symbol. If nLeftPos == Length or -1, items are appended. nLeftPos > Length is not allowed
 * \param pWcharSource - pointer to "wide"-characters string to insert. String is returned by mdz_wchar_create() or mdz_wchar_create_attached()
 * \param bReserve - if mdz_true reserve capacity when there is not enough space for insertion, otherwise mdz_false
 * \param pAsyncData - pointer to shared async data for asynchronous call, or NULL if call should be synchronous
 * \return:
 * mdz_false - if pUtf8 == NULL
 * mdz_false - if bReserve == mdz_true and memory allocation failed (MDZ_ERROR_ALLOCATION)
 * mdz_false - if bReserve == mdz_true and there is not enough capacity for inserted data, but m_pData is attached using mdz_utf8_attachData() (MDZ_ERROR_ATTACHED)
 * mdz_false - if bReserve == mdz_false and there is not enough free Capacity in the string (MDZ_ERROR_CAPACITY)
 * mdz_true  - if pWcharSource == NULL (MDZ_ERROR_SOURCE), or pWcharSource.Size == 0 (MDZ_ERROR_ZEROCOUNT), or nLeftPos > Length (MDZ_ERROR_BIGLEFT). No insertion is made
 * mdz_true  - insertion succeeded
 */
mdz_bool mdz_utf8_insertWchar_string_async(struct mdz_Utf8* pUtf8, size_t nLeftPos, const struct mdz_Wchar* pWcharSource, mdz_bool bReserve, struct mdz_asyncData* pAsyncData);

/**
 * Synchronous version
 */
#define mdz_utf8_insertWchar_string(pUtf8, nLeftPos, pWcharSource, bReserve) mdz_utf8_insertWchar_string_async(pUtf8, nLeftPos, pWcharSource, bReserve, NULL)

/**
 * Insert nCount UTF-16 characters in string. UTF-16 characters are converted to UTF-8 before isertion. Size grows on added bytes. Length grows on symbols count.
 * UTF-16 "surrogate pairs" are supported and counted as 1 symbol (4 bytes size).
 * UTF-16 "combining characters" are not specially-distinguished and counted as a separate UTF-8 symbol.
 * \param pUtf8 - pointer to string returned by mdz_utf8_create() or mdz_utf8_create_attached()
 * \param nLeftPos - 0-based position to insert in symbols. If nLeftPos == Length or -1, items are appended. nLeftPos > Length is not allowed
 * \param pItems - UTF-16 characters to insert
 * \param nCount - number of UTF-16 characters to insert or 0 if pcItems until 0-terminator should be used
 * \param enEndianness - endianness of UTF-16 characters in pItems. Can be MDZ_ENDIAN_LITTLE for "little-endian" or MDZ_ENDIAN_BIG for "big-endian"
 * \param bReserve - if mdz_true reserve capacity when there is not enough space for insertion, otherwise mdz_false
 * \param pAsyncData - pointer to shared async data for asynchronous call, or NULL if call should be synchronous
 * \return:
 * mdz_false - if pUtf8 == NULL
 * mdz_false - if bReserve == mdz_true and memory allocation failed (MDZ_ERROR_ALLOCATION)
 * mdz_false - if bReserve == mdz_true and there is not enough capacity for inserted data, but m_pData is attached using mdz_utf8_attachData() (MDZ_ERROR_ATTACHED)
 * mdz_false - if bReserve == mdz_false and there is not enough free Capacity in the string (MDZ_ERROR_CAPACITY)
 * mdz_false - if pItems contain invalid UTF-16 character(s) (MDZ_ERROR_CONTENT), or invalid enEndianness (MDZ_ERROR_ENDIANNESS)
 * mdz_true  - if pItems == NULL (MDZ_ERROR_ITEMS), or nCount == 0 (MDZ_ERROR_ZEROCOUNT), or nLeftPos > Length (MDZ_ERROR_BIGLEFT). No insertion is made
 * mdz_true  - insertion succeeded
 */
mdz_bool mdz_utf8_insertUtf16_async(struct mdz_Utf8* pUtf8, size_t nLeftPos, const uint16_t* pItems, size_t nCount, enum mdz_endianness enEndianness, mdz_bool bReserve, struct mdz_asyncData* pAsyncData);

/**
 * Synchronous version
 */
#define mdz_utf8_insertUtf16(pUtf8, nLeftPos, pItems, nCount, enEndianness, bReserve) mdz_utf8_insertUtf16_async(pUtf8, nLeftPos, pItems, nCount, enEndianness, bReserve, NULL)

/**
 * Insert UTF-16 string pUtf16Source in string. UTF-16 characters are converted to UTF-8 before isertion. Size grows on added bytes. Length grows on symbols count.
 * UTF-16 "surrogate pairs" are supported and counted as 1 symbol (4 bytes size).
 * UTF-16 "combining characters" are not specially-distinguished and counted as a separate UTF-8 symbol.
 * \param pUtf8 - pointer to string returned by mdz_utf8_create() or mdz_utf8_create_attached()
 * \param nLeftPos - 0-based position to insert in symbols. If nLeftPos == Length or -1, items are appended. nLeftPos > Length is not allowed
 * \param pUtf16Source - pointer to UTF16 string to insert. String is returned by mdz_utf16_create() or mdz_utf16_create_attached()
 * \param bReserve - if mdz_true reserve capacity when there is not enough space for insertion, otherwise mdz_false
 * \param pAsyncData - pointer to shared async data for asynchronous call, or NULL if call should be synchronous
 * \return:
 * mdz_false - if pUtf8 == NULL
 * mdz_false - if bReserve == mdz_true and memory allocation failed (MDZ_ERROR_ALLOCATION)
 * mdz_false - if bReserve == mdz_true and there is not enough capacity for inserted data, but m_pData is attached using mdz_utf8_attachData() (MDZ_ERROR_ATTACHED)
 * mdz_false - if bReserve == mdz_false and there is not enough free Capacity in the string (MDZ_ERROR_CAPACITY)
 * mdz_true  - if pUtf16Source == NULL (MDZ_ERROR_SOURCE), or pUtf16Source.Size == 0 (MDZ_ERROR_ZEROCOUNT), or nLeftPos > Length (MDZ_ERROR_BIGLEFT). No insertion is made
 * mdz_true  - insertion succeeded
 */
mdz_bool mdz_utf8_insertUtf16_string_async(struct mdz_Utf8* pUtf8, size_t nLeftPos, const struct mdz_Utf16* pUtf16Source, mdz_bool bReserve, struct mdz_asyncData* pAsyncData);

/**
 * Synchronous version
 */
#define mdz_utf8_insertUtf16_string(pUtf8, nLeftPos, pUtf16Source, bReserve) mdz_utf8_insertUtf16_string_async(pUtf8, nLeftPos, pUtf16Source, bReserve, NULL)

/**
 * Insert nCount UTF-32 characters in string. UTF-32 characters are converted to UTF-8 before isertion. Size grows on added bytes. Length grows on symbols count.
 * Unicode "combining characters" are not specially-distinguished and counted as a separate UTF-8 symbol.
 * \param pUtf8 - pointer to string returned by mdz_utf8_create() or mdz_utf8_create_attached()
 * \param nLeftPos - 0-based position to insert in symbols. If nLeftPos == Length or -1, items are appended. nLeftPos > Length is not allowed
 * \param pItems - UTF-32 characters to insert
 * \param nCount - number of UTF-32 characters to insert or 0 if pcItems until 0-terminator should be used
 * \param enEndianness - endianness of UTF-32 characters in pItems. Can be MDZ_ENDIAN_LITTLE for "little-endian" or MDZ_ENDIAN_BIG for "big-endian"
 * \param bReserve - if mdz_true reserve capacity when there is not enough space for insertion, otherwise mdz_false
 * \param pAsyncData - pointer to shared async data for asynchronous call, or NULL if call should be synchronous
 * \return:
 * mdz_false - if pUtf8 == NULL
 * mdz_false - if bReserve == mdz_true and memory allocation failed (MDZ_ERROR_ALLOCATION)
 * mdz_false - if bReserve == mdz_true and there is not enough capacity for inserted data, but m_pData is attached using mdz_utf8_attachData() (MDZ_ERROR_ATTACHED)
 * mdz_false - if bReserve == mdz_false and there is not enough free Capacity in the string (MDZ_ERROR_CAPACITY)
 * mdz_true  - if pItems == NULL (MDZ_ERROR_ITEMS), or nCount == 0 (MDZ_ERROR_ZEROCOUNT), or nLeftPos > Length (MDZ_ERROR_BIGLEFT). No insertion is made
 * mdz_true  - insertion succeeded
 */
mdz_bool mdz_utf8_insertUtf32_async(struct mdz_Utf8* pUtf8, size_t nLeftPos, const uint32_t* pItems, size_t nCount, enum mdz_endianness enEndianness, mdz_bool bReserve, struct mdz_asyncData* pAsyncData);

/**
 * Synchronous version
 */
#define mdz_utf8_insertUtf32(pUtf8, nLeftPos, pItems, nCount, enEndianness, bReserve) mdz_utf8_insertUtf32_async(pUtf8, nLeftPos, pItems, nCount, enEndianness, bReserve, NULL)

/**
 * Insert UTF-32 string pUtf32Source in string. UTF-32 characters are converted to UTF-8 before isertion. Size grows on added bytes. Length grows on symbols count.
 * Unicode "combining characters" are not specially-distinguished and counted as a separate UTF-8 symbol.
 * \param pUtf8 - pointer to string returned by mdz_utf8_create() or mdz_utf8_create_attached()
 * \param nLeftPos - 0-based position to insert in symbols. If nLeftPos == Length or -1, items are appended. nLeftPos > Length is not allowed
 * \param pUtf32Source - pointer to UTF-32 string to insert. String is returned by mdz_utf32_create() or mdz_utf32_create_attached()
 * \param bReserve - if mdz_true reserve capacity when there is not enough space for insertion, otherwise mdz_false
 * \param pAsyncData - pointer to shared async data for asynchronous call, or NULL if call should be synchronous
 * \return:
 * mdz_false - if pUtf8 == NULL
 * mdz_false - if bReserve == mdz_true and memory allocation failed (MDZ_ERROR_ALLOCATION)
 * mdz_false - if bReserve == mdz_true and there is not enough capacity for inserted data, but m_pData is attached using mdz_utf8_attachData() (MDZ_ERROR_ATTACHED)
 * mdz_false - if bReserve == mdz_false and there is not enough free Capacity in the string (MDZ_ERROR_CAPACITY)
 * mdz_true  - if pUtf32Source == NULL (MDZ_ERROR_SOURCE), or pUtf32Source.Size == 0 (MDZ_ERROR_ZEROCOUNT), or nLeftPos > Length (MDZ_ERROR_BIGLEFT). No insertion is made
 * mdz_true  - insertion succeeded
 */
mdz_bool mdz_utf8_insertUtf32_string_async(struct mdz_Utf8* pUtf8, size_t nLeftPos, const struct mdz_Utf32* pUtf32Source, mdz_bool bReserve, struct mdz_asyncData* pAsyncData);

/**
 * Synchronous version
 */
#define mdz_utf8_insertUtf32_string(pUtf8, nLeftPos, pUtf32Source, bReserve) mdz_utf8_insertUtf32_string_async(pUtf8, nLeftPos, pUtf32Source, bReserve, NULL)

#ifdef __cplusplus
}
#endif

#endif
