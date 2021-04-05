/**
 * \ingroup mdz_unicode library
 *
 * \author maxdz Software GmbH
 *
 * \par license
 * This file is subject to the terms and conditions defined in file 'LICENSE.txt', which is part of this source code package.
 *
 * \par description
 * mdz_utf32 is dynamically-sized contiguous string, containing UTF-32 characters.
 *
 * Capacity - how many UTF-32 characters memory is reserved for.
 * Size - how many UTF-32 characters are actually used in a string, excluding terminating 0.
 * Length - actual length of string in symbols, excluding terminating 0.
 *
 * "reserve/AndReserve" functions allocate/reallocate memory dynamically using malloc()/realloc().
 * "attach" functionality allows attaching contiguous block of memory to string, for using string functions on it.
 *
 * Unicode "combining characters" are not specially-distinguished and counted as distinct symbols
 * Endianness of bytes in UTF-32 character - is defined by m_enEndianness and set using enEndianness during string-creation in mdz_utf32_create()
 *
 * \par info
 * See additional info on mdz_unicode library like version, portability, etc in mdz_unicode.h
 */

#ifndef MDZ_UNICODE_UTF32_H
#define MDZ_UNICODE_UTF32_H

#include "mdz_types.h"

#include <stdint.h>

/**
 * Handle of UTF-32 string in mdz_unicode library
 */
struct mdz_Utf32
{
  /**
   * Pointer to data of string. Do not change it directly.
   */
  const uint32_t* m_pData;

  /**
   * Error code if operation failed or there are some inconsistences.
   */
  enum mdz_error m_enErrorCode;
};

#ifdef __cplusplus
extern "C" 
{
#endif

/**
 * \defgroup Init and destroy functions
 */

/**
 * Create empty UTF-32 string with Capacity == 1 (for 0-terminator), Size == 0 and Length == 0.
 * \param nEmbedSize - size of "embedded part" of string. There is no "embedded part" if 0
 * \param enEndianness - endianness of string. Should be MDZ_ENDIAN_LITTLE or MDZ_ENDIAN_BIG
 * \return:
 * NULL   - if library is not initialized with mdz_unicode_init() call
 * NULL   - if memory allocation failed
 * NULL   - if invalid enEndianness
 * Result - pointer to string for use in other mdz_utf32 functions
 */
struct mdz_Utf32* mdz_utf32_create(size_t nEmbedSize, enum mdz_endianness enEndianness);

/**
 * Create empty UTF-32 string with Capacity == 1 (for 0-terminator), Size == 0 and Length == 0.
 * Memory for utf32 structure starts at position pStart. Size of internal utf32 structure (it is usually bigger than mdz_Utf32!) is returned in pSize.
 * \param pStart - memory start position of utf32 structure
 * \param enEndianness - endianness of string. Should be MDZ_ENDIAN_LITTLE or MDZ_ENDIAN_BIG
 * \param nAreaSizeBytes - size of available memory from pStart in bytes. Should be large enough for internal utf32 structure
 * \param pOutSize - returned actual size of allocated internal utf32 structure in bytes, may be NULL if not needed
 * \return:
 * NULL   - if library is not initialized with mdz_unicode_init() call
 * NULL   - if invalid enEndianness
 * NULL   - if pStart == NULL or pSize == NULL
 * NULL   - if size in nSize is smaller than size of internal utf32 structure
 * Result - pointer to string for use in other mdz_utf32 functions. Normally it equals to pStart
 */
struct mdz_Utf32* mdz_utf32_create_attached(const void* pStart, enum mdz_endianness enEndianness, size_t nAreaSizeBytes, size_t* pOutSize);

/**
 * Destroy UTF-32 string including underlying data. After destroying, pointer to string is set to NULL.
 * If utf32 is attached using mdz_utf32_createAttached(), free() will not be called.
 * If utf32 data is attached using mdz_utf32_attachData(), m_pData will not be destroyed.
 * \param ppUtf32 - pointer to pointer to string returned by mdz_utf32_create() or mdz_utf32_create_attached()
 */
void mdz_utf32_destroy(struct mdz_Utf32** ppUtf32);

/**
 * Clear m_pData of UTF-32 string with setting Size in 0.
 * \param pUtf32 - pointer to string returned by mdz_utf32_create()
 */
void mdz_utf32_clear(struct mdz_Utf32* pUtf32);

/**
 * Attach pre-allocated data to UTF-32 string, assigning pData to m_pData. If attached, m_pData will not be destroyed in mdz_utf32_destroy()
 * \param pUtf32 - pointer to string returned by mdz_utf32_create() or mdz_utf32_create_attached()
 * \param pData - pointer to pre-allocated data to attach
 * \param nOffsetFromStart - position in pre-allocated data to attach from. Can be > 0
 * \param nCapacity - full capacity pre-allocated data in items
 * \param enAttachType - type of attachment. 0 is expected at position pData[nOffsetFromStart] if MDZ_ATTACH_ZEROSIZE. 0 is expected at position pData[nCapacity] if MDZ_ATTACH_SIZE_TERMINATOR. MDZ_ATTACH_SIZE_NO_TERMINATOR is not allowed
 * \param enEndianness - endianness of UTF-32 characters in pData. Can be MDZ_ENDIAN_LITTLE for "little-endian" or MDZ_ENDIAN_BIG for "big-endian"
 * \return:
 * mdz_false - if pUtf32 == NULL
 * mdz_false - if pData == NULL (MDZ_ERROR_DATA), or nOffsetFromStart >= nCapacity (MDZ_ERROR_OFFSET), or invalid enAttachType (MDZ_ERROR_ATTACHTYPE), or invalid pData endianness (MDZ_ERROR_ENDIANNESS)
 * mdz_false - if enAttachType is MDZ_ATTACH_ZEROSIZE or MDZ_ATTACH_SIZE_TERMINATOR but 0 is not found at expected position (MDZ_ERROR_ATTACH_TERMINATOR)
 * mdz_true  - operation succeeded
 */
mdz_bool mdz_utf32_attachData(struct mdz_Utf32* pUtf32, uint32_t* pData, size_t nOffsetFromStart, size_t nCapacity, enum mdz_attach_type enAttachType, enum mdz_endianness enEndianness);

/**
 * \defgroup Reserve capacity functions
 */

/**
 * Reserve nNewCapacity bytes for UTF-32 string. Size and Length do not change.
 * \param pUtf32 - pointer to string returned by mdz_utf32_create() or mdz_utf32_create_attached()
 * \param nNewCapacity - new capacity in UTF-32 characters to reserve
 * \return:
 * mdz_false - if pUtf32 == NULL
 * mdz_false - if memory allocation failed (MDZ_ERROR_ALLOCATION)
 * mdz_true	 - reservation succeeded, or nNewCapacity <= Capacity (MDZ_ERROR_CAPACITY)
 */
mdz_bool mdz_utf32_reserve(struct mdz_Utf32* pUtf32, size_t nNewCapacity);

/**
 * Return string Capacity in UTF-32 characters.
 * \param pUtf32 - pointer to string returned by mdz_utf32_create() or mdz_utf32_create_attached()
 * \return:
 * SIZE_MAX - if pUtf32 == NULL
 * Capacity - otherwise
 */
size_t mdz_utf32_capacity(const struct mdz_Utf32* pUtf32);

/**
 * Return string Size in UTF-32 characters.
 * \param pUtf32 - pointer to string returned by mdz_utf32_create() or mdz_utf32_create_attached()
 * \return:
 * SIZE_MAX - if pUtf32 == NULL
 * Size     - otherwise
 */
size_t mdz_utf32_size(const struct mdz_Utf32* pUtf32);

/**
 * Return string length in symbols.
 * \param pUtf32 - pointer to string returned by mdz_utf32_create() or mdz_utf32_create_attached()
 * \return:
 * SIZE_MAX - if pUtf32 == NULL
 * Size     - otherwise
 */
size_t mdz_utf32_length(const struct mdz_Utf32* pUtf32);

/**
 * Return string endianness.
 * \param pUtf32 - pointer to string returned by mdz_utf32_create() or mdz_utf32_create_attached()
 * \return:
 * MDZ_ENDIAN_ERROR - if pUtf32 == NULL
 * Endianness       - otherwise
 */
enum mdz_endianness mdz_utf32_endianness(const struct mdz_Utf32* pUtf32);

/**
 * Return string OffsetFromStart in UTF-32 characters.
 * \param pUtf32 - pointer to string returned by mdz_utf32_create() or mdz_utf32_create_attached()
 * \return:
 * SIZE_MAX        - if pUtf32 == NULL
 * OffsetFromStart - otherwise
*/
size_t mdz_utf32_offsetFromStart(const struct mdz_Utf32* pUtf32);

/**
 * Return if string data is attached.
 * \param pUtf32 - pointer to string returned by mdz_utf32_create() or mdz_utf32_create_attached()
 * \return:
 * mdz_false - if pUtf32 == NULL
 * mdz_false - if string data is not attached
 * mdz_true  - if string data is attached
*/
mdz_bool mdz_utf32_isAttachedData(const struct mdz_Utf32* pUtf32);

/**
 * Return string "embedded part" Size in UTF-32 characters.
 * \param pUtf32 - pointer to string returned by mdz_utf32_create() or mdz_utf32_create_attached()
 * \return:
 * SIZE_MAX - if pUtf32 == NULL
 * Result   - "embedded part" Size otherwise
 */
size_t mdz_utf32_embedSize(const struct mdz_Utf32* pUtf32);

/**
 * \defgroup Insert/remove functions
 */

/**
 * Insert nCount UTF-32 characters in string. Size grows on nCount. Length grows on symbols count.
 * String m_pData and pItems should not overlap.
 * \param pUtf32 - pointer to string returned by mdz_utf32_create() or mdz_utf32_create_attached()
 * \param nLeftPos - 0-based position to insert in symbols. If nLeftPos == Length or -1, items are appended. nLeftPos > Length is not allowed
 * \param pItems - UTF-32 characters to insert
 * \param nCount - number of UTF-32 characters to insert or 0 if pcItems until 0-terminator should be used
 * \param enEndianness - endianness of UTF-32 characters in pItems. Can be MDZ_ENDIAN_LITTLE for "little-endian" or MDZ_ENDIAN_BIG for "big-endian"
 * \param bReserve - if mdz_true reserve capacity when there is not enough space for insertion, otherwise mdz_false
 * \param pAsyncData - pointer to shared async data for asynchronous call, or NULL if call should be synchronous
 * \return:
 * mdz_false - if pUtf32 == NULL
 * mdz_false - if bReserve == mdz_true and memory allocation failed (MDZ_ERROR_ALLOCATION)
 * mdz_false - if bReserve == mdz_true and there is not enough capacity for inserted data, but m_pData is attached using mdz_utf32_attachData() (MDZ_ERROR_ATTACHED)
 * mdz_false - if bReserve == mdz_false and there is not enough free Capacity in the string (MDZ_ERROR_CAPACITY)
 * mdz_false - if m_pData and pItems overlap (MDZ_ERROR_OVERLAP)
 * mdz_true  - if pItems == NULL (MDZ_ERROR_ITEMS), or nCount == 0 (MDZ_ERROR_ZEROCOUNT), or nLeftPos > Length (MDZ_ERROR_BIGLEFT). No insertion is made
 * mdz_true  - insertion succeeded
 */
mdz_bool mdz_utf32_insertUtf32_async(struct mdz_Utf32* pUtf32, size_t nLeftPos, const uint32_t* pItems, size_t nCount, enum mdz_endianness enEndianness, mdz_bool bReserve, struct mdz_asyncData* pAsyncData);

/**
 * Synchronous version
 */
#define mdz_utf32_insertUtf32(pUtf32, nLeftPos, pItems, nCount, enEndianness, bReserve) mdz_utf32_insertUtf32_async(pUtf32, nLeftPos, pItems, nCount, enEndianness, bReserve, NULL)

/**
 * Insert UTF-32 string pUtf32Source in string. Size grows on nCount. Length grows on symbols count.
 * String m_pData and pUtf32Source->m_pData should not overlap.
 * This function performs significantly better than mdz_utf32_insertUtf32_async() - because there is no additional validation of inserted string.
 * \param pUtf32 - pointer to string returned by mdz_utf32_create() or mdz_utf32_create_attached()
 * \param nLeftPos - 0-based position to insert in symbols. If nLeftPos == Length or -1, items are appended. nLeftPos > Length is not allowed
 * \param pUtf32Source - pointer to UTF-32 string to insert. String is returned by mdz_utf32_create() or mdz_utf32_create_attached()
 * \param bReserve - if mdz_true reserve capacity when there is not enough space for insertion, otherwise mdz_false
 * \param pAsyncData - pointer to shared async data for asynchronous call, or NULL if call should be synchronous
 * \return:
 * mdz_false - if pUtf32 == NULL
 * mdz_false - if bReserve == mdz_true and memory allocation failed (MDZ_ERROR_ALLOCATION)
 * mdz_false - if bReserve == mdz_true and there is not enough capacity for inserted data, but m_pData is attached using mdz_utf32_attachData() (MDZ_ERROR_ATTACHED)
 * mdz_false - if bReserve == mdz_false and there is not enough free Capacity in the string (MDZ_ERROR_CAPACITY)
 * mdz_false - if invalid enEndianness (MDZ_ERROR_ENDIANNESS)
 * mdz_false - if m_pData and pUtf32Source->m_pData overlap (MDZ_ERROR_OVERLAP)
 * mdz_true  - if pUtf32Source == NULL (MDZ_ERROR_SOURCE), or pUtf32Source.Size == 0 (MDZ_ERROR_ZEROCOUNT), or nLeftPos > Length (MDZ_ERROR_BIGLEFT). No insertion is made
 * mdz_true  - insertion succeeded
 */
mdz_bool mdz_utf32_insertUtf32_string_async(struct mdz_Utf32* pUtf32, size_t nLeftPos, const struct mdz_Utf32* pUtf32Source, mdz_bool bReserve, struct mdz_asyncData* pAsyncData);

/**
 * Synchronous version
 */
#define mdz_utf32_insertUtf32_string(pUtf32, nLeftPos, pUtf32Source, bReserve) mdz_utf32_insertUtf32_string_async(pUtf32, nLeftPos, pUtf32Source, bReserve, NULL)

/**
 * Insert nCount ASCII/ANSI bytes in string. Characters are converted to UTF-32 characters before isertion. Size grows on added UTF-32 characters count. Length grows on added symbols count.
 * String m_pData and pcItems should not overlap.
 * \param pUtf32 - pointer to string returned by mdz_utf32_create() or mdz_utf32_create_attached()
 * \param nLeftPos - 0-based position to insert in symbols. If nLeftPos == Length or -1, items are appended. nLeftPos > Length is not allowed
 * \param pcItems - bytes to insert
 * \param nCount - number of bytes to insert or 0 if pcItems until 0-terminator should be used
 * \param bReserve - if mdz_true reserve capacity when there is not enough space for insertion, otherwise mdz_false
 * \param pAsyncData - pointer to shared async data for asynchronous call, or NULL if call should be synchronous
 * \return:
 * mdz_false - if pUtf32 is NULL
 * mdz_false - if bReserve == mdz_true and memory allocation failed (MDZ_ERROR_ALLOCATION)
 * mdz_false - if bReserve == mdz_true and there is not enough capacity for inserted data, but m_pData is attached using mdz_utf32_attachData() (MDZ_ERROR_ATTACHED)
 * mdz_false - if bReserve == mdz_false and there is not enough free Capacity in the string (MDZ_ERROR_CAPACITY)
 * mdz_false - if m_pData and pcItems overlap (MDZ_ERROR_OVERLAP)
 * mdz_true  - if pcItems == NULL (MDZ_ERROR_ITEMS), or nCount == 0 (MDZ_ERROR_ZEROCOUNT), or nLeftPos > Length (MDZ_ERROR_BIGLEFT). No insertion is made
 * mdz_true  - insertion succeeded
 */
mdz_bool mdz_utf32_insertAnsi_async(struct mdz_Utf32* pUtf32, size_t nLeftPos, const unsigned char* pcItems, size_t nCount, mdz_bool bReserve, struct mdz_asyncData* pAsyncData);

/**
 * Synchronous version
 */
#define mdz_utf32_insertAnsi(pUtf32, nLeftPos, pcItems, nCount, bReserve) mdz_utf32_insertAnsi_async(pUtf32, nLeftPos, pcItems, nCount, bReserve, NULL)

/**
 * Insert ASCII/ANSI string pAnsiSource in string. Characters are converted to UTF-32 characters before isertion. Size grows on added UTF-32 characters count. Length grows on added symbols count.
 * String m_pData and pAnsiSource->m_pData should not overlap.
 * \param pUtf32 - pointer to string returned by mdz_utf32_create() or mdz_utf32_create_attached()
 * \param nLeftPos - 0-based position to insert in symbols. If nLeftPos == Length or -1, items are appended. nLeftPos > Length is not allowed
 * \param pAnsiSource - pointer to ASCII/ANSI string to insert. String is returned by mdz_ansi_create() or mdz_ansi_create_attached()
 * \param bReserve - if mdz_true reserve capacity when there is not enough space for insertion, otherwise mdz_false
 * \param pAsyncData - pointer to shared async data for asynchronous call, or NULL if call should be synchronous
 * \return:
 * mdz_false - if pUtf32 is NULL
 * mdz_false - if bReserve == mdz_true and memory allocation failed (MDZ_ERROR_ALLOCATION)
 * mdz_false - if bReserve == mdz_true and there is not enough capacity for inserted data, but m_pData is attached using mdz_utf32_attachData() (MDZ_ERROR_ATTACHED)
 * mdz_false - if bReserve == mdz_false and there is not enough free Capacity in the string (MDZ_ERROR_CAPACITY)
 * mdz_false - if m_pData and pAnsiSource->m_pData overlap (MDZ_ERROR_OVERLAP)
 * mdz_true  - if pAnsiSource == NULL (MDZ_ERROR_SOURCE), or pAnsiSource.Size == 0 (MDZ_ERROR_ZEROCOUNT), or nLeftPos > Length (MDZ_ERROR_BIGLEFT). No insertion is made
 * mdz_true  - insertion succeeded
 */
mdz_bool mdz_utf32_insertAnsi_string_async(struct mdz_Utf32* pUtf32, size_t nLeftPos, const void* pAnsiSource, mdz_bool bReserve, struct mdz_asyncData* pAsyncData);

/**
 * Synchronous version
 */
#define mdz_utf32_insertAnsi_string(pUtf32, nLeftPos, pAnsiSource, bReserve) mdz_utf32_insertAnsi_string_async(pUtf32, nLeftPos, pAnsiSource, bReserve, NULL)

/**
 * Insert nCount "wide"-characters in string. Characters are converted to UTF-32 characters before isertion. Size grows on added UTF-32 characters count. Length grows on added symbols count.
 * String m_pData and pwcItems should not overlap.
 * \param pUtf32 - pointer to string returned by mdz_utf32_create() or mdz_utf32_create_attached()
 * \param nLeftPos - 0-based position to insert in symbols symbols. If nLeftPos == Length or -1, items are appended. nLeftPos > Length is not allowed
 * \param pwcItems - "wide"-characters to insert
 * \param nCount - number of characters to insert in bytes or 0 if pcItems until 0-terminator should be used
 * \param nWcharSize - size of pwcItems wchar_t character in bytes
 * \param bReserve - if mdz_true reserve capacity when there is not enough space for insertion, otherwise mdz_false
 * \param pAsyncData - pointer to shared async data for asynchronous call, or NULL if call should be synchronous
 * \return:
 * mdz_false - if pUtf32 is NULL
 * mdz_false - if bReserve == mdz_true and memory allocation failed (MDZ_ERROR_ALLOCATION)
 * mdz_false - if nWcharSize is not 2 or 4 (MDZ_ERROR_WCHAR_SIZE)
 * mdz_false - if bReserve == mdz_true and there is not enough capacity for inserted data, but m_pData is attached using mdz_utf32_attachData() (MDZ_ERROR_ATTACHED)
 * mdz_false - if bReserve == mdz_false and there is not enough free Capacity in the string (MDZ_ERROR_CAPACITY)
 * mdz_false - if m_pData and pwcItems overlap (MDZ_ERROR_OVERLAP)
 * mdz_true  - if pcItems == NULL (MDZ_ERROR_ITEMS), or nCount == 0 (MDZ_ERROR_ZEROCOUNT), or nLeftPos > Length (MDZ_ERROR_BIGLEFT). No insertion is made
 * mdz_true  - insertion succeeded
 */
mdz_bool mdz_utf32_insertWchar_async(struct mdz_Utf32* pUtf32, size_t nLeftPos, const wchar_t* pwcItems, size_t nCount, size_t nWcharSize, mdz_bool bReserve, struct mdz_asyncData* pAsyncData);

/**
 * Synchronous version
 */
#define mdz_utf32_insertWchar(pUtf32, nLeftPos, pwcItems, nCount, nWcharSize, bReserve) mdz_utf32_insertWchar_async(pUtf32, nLeftPos, pwcItems, nCount, nWcharSize, bReserve, NULL)

/**
 * Insert "wide"-characters string pWcharSource in string. Characters are converted to UTF-32 characters before isertion. Size grows on added UTF-32 characters count. Length grows on added symbols count.
 * String m_pData and pWcharSource->m_pData should not overlap.
 * \param pUtf32 - pointer to string returned by mdz_utf32_create() or mdz_utf32_create_attached()
 * \param nLeftPos - 0-based position to insert in symbols symbols. If nLeftPos == Length or -1, items are appended. nLeftPos > Length is not allowed
 * \param pWcharSource - pointer to "wide"-characters string to insert. String is returned by mdz_wchar_create() or mdz_wchar_create_attached()
 * \param bReserve - if mdz_true reserve capacity when there is not enough space for insertion, otherwise mdz_false
 * \param pAsyncData - pointer to shared async data for asynchronous call, or NULL if call should be synchronous
 * \return:
 * mdz_false - if pUtf32 is NULL
 * mdz_false - if bReserve == mdz_true and memory allocation failed (MDZ_ERROR_ALLOCATION)
 * mdz_false - if bReserve == mdz_true and there is not enough capacity for inserted data, but m_pData is attached using mdz_utf32_attachData() (MDZ_ERROR_ATTACHED)
 * mdz_false - if bReserve == mdz_false and there is not enough free Capacity in the string (MDZ_ERROR_CAPACITY)
 * mdz_false - if m_pData and pWcharSource->m_pData overlap (MDZ_ERROR_OVERLAP)
 * mdz_true  - if pWcharSource == NULL (MDZ_ERROR_SOURCE), or pWcharSource.Size == 0 (MDZ_ERROR_ZEROCOUNT), or nLeftPos > Length (MDZ_ERROR_BIGLEFT). No insertion is made
 * mdz_true  - insertion succeeded
 */
mdz_bool mdz_utf32_insertWchar_string_async(struct mdz_Utf32* pUtf32, size_t nLeftPos, const void* pWcharSource, mdz_bool bReserve, struct mdz_asyncData* pAsyncData);

/**
 * Synchronous version
 */
#define mdz_utf32_insertWchar_string(pUtf32, nLeftPos, pWcharSource, bReserve) mdz_utf32_insertWchar_string_async(pUtf32, nLeftPos, pWcharSource, bReserve, NULL)

/**
 * Insert nCount UTF-8 characters in string. UTF-8 characters are converted to UTF-32 characters before isertion. Size grows on added UTF-32 characters count. Length grows on added symbols count.
 * String m_pData and pcItems should not overlap.
 * \param pUtf32 - pointer to string returned by mdz_utf32_create() or mdz_utf32_create_attached()
 * \param nLeftPos - 0-based position to insert in symbols. If nLeftPos == Length or -1, items are appended. nLeftPos > Length is not allowed
 * \param pcItems - UTF-8 characters to insert
 * \param nCount - number of UTF-8 characters to insert in bytes or 0 if pcItems until 0-terminator should be used
 * \param bReserve - if mdz_true reserve capacity when there is not enough space for insertion, otherwise mdz_false
 * \param pAsyncData - pointer to shared async data for asynchronous call, or NULL if call should be synchronous
 * \return:
 * mdz_false - if pUtf32 is NULL
 * mdz_false - if bReserve == mdz_true and memory allocation failed (MDZ_ERROR_ALLOCATION)
 * mdz_false - if bReserve == mdz_true and there is not enough capacity for inserted data, but m_pData is attached using mdz_utf32_attachData() (MDZ_ERROR_ATTACHED)
 * mdz_false - if bReserve == mdz_false and there is not enough free Capacity in the string (MDZ_ERROR_CAPACITY)
 * mdz_false - if pcItems contain invalid UTF-8 character(s) (MDZ_ERROR_CONTENT)
 * mdz_false - if m_pData and pcItems overlap (MDZ_ERROR_OVERLAP)
 * mdz_true  - if pcItems == NULL (MDZ_ERROR_ITEMS), or nCount == 0 (MDZ_ERROR_ZEROCOUNT), or nLeftPos > Length (MDZ_ERROR_BIGLEFT). No insertion is made
 * mdz_true  - insertion succeeded
 */
mdz_bool mdz_utf32_insertUtf8_async(struct mdz_Utf32* pUtf32, size_t nLeftPos, const unsigned char* pcItems, size_t nCount, mdz_bool bReserve, struct mdz_asyncData* pAsyncData);

/**
 * Synchronous version
 */
#define mdz_utf32_insertUtf8(pUtf32, nLeftPos, pcItems, nCount, bReserve) mdz_utf32_insertUtf8_async(pUtf32, nLeftPos, pcItems, nCount, bReserve, NULL)

/**
 * Insert UTF-8 string pUtf8Source in string. UTF-8 characters are converted to UTF-32 characters before isertion. Size grows on added UTF-32 characters count. Length grows on added symbols count.
 * String m_pData and pUtf8Source->m_pData should not overlap.
 * \param pUtf32 - pointer to string returned by mdz_utf32_create() or mdz_utf32_create_attached()
 * \param nLeftPos - 0-based position to insert in symbols. If nLeftPos == Length or -1, items are appended. nLeftPos > Length is not allowed
 * \param pUtf8Source - pointer to UTF-8 string to insert. String is returned by mdz_utf8_create() or mdz_utf8_create_attached()
 * \param bReserve - if mdz_true reserve capacity when there is not enough space for insertion, otherwise mdz_false
 * \param pAsyncData - pointer to shared async data for asynchronous call, or NULL if call should be synchronous
 * \return:
 * mdz_false - if pUtf32 is NULL
 * mdz_false - if bReserve == mdz_true and memory allocation failed (MDZ_ERROR_ALLOCATION)
 * mdz_false - if bReserve == mdz_true and there is not enough capacity for inserted data, but m_pData is attached using mdz_utf32_attachData() (MDZ_ERROR_ATTACHED)
 * mdz_false - if bReserve == mdz_false and there is not enough free Capacity in the string (MDZ_ERROR_CAPACITY)
 * mdz_false - if m_pData and pUtf8Source->m_pData overlap (MDZ_ERROR_OVERLAP)
 * mdz_true  - if pUtf8Source == NULL (MDZ_ERROR_SOURCE), or pUtf8Source.Size == 0 (MDZ_ERROR_ZEROCOUNT), or nLeftPos > Length (MDZ_ERROR_BIGLEFT). No insertion is made
 * mdz_true  - insertion succeeded
 */
mdz_bool mdz_utf32_insertUtf8_string_async(struct mdz_Utf32* pUtf32, size_t nLeftPos, const void* pUtf8Source, mdz_bool bReserve, struct mdz_asyncData* pAsyncData);

/**
 * Synchronous version
 */
#define mdz_utf32_insertUtf8_string(pUtf32, nLeftPos, pUtf8Source, bReserve) mdz_utf32_insertUtf8_string_async(pUtf32, nLeftPos, pUtf8Source, bReserve, NULL)

/**
 * Insert nCount UTF-16 characters in string. Characters are converted to UTF-32 characters before isertion. Size grows on added UTF-32 characters count. Length grows on symbols count.
 * String m_pData and pItems should not overlap.
 * \param pUtf32 - pointer to string returned by mdz_utf32_create() or mdz_utf32_create_attached()
 * \param nLeftPos - 0-based position to insert in symbols. If nLeftPos == Length or -1, items are appended. nLeftPos > Length is not allowed
 * \param pItems - UTF-16 items to insert
 * \param nCount - number of UTF-16 items to insert or 0 if pcItems until 0-terminator should be used
 * \param enEndianness - endianness of UTF-16 characters in pItems. Can be MDZ_ENDIAN_LITTLE for "little-endian" or MDZ_ENDIAN_BIG for "big-endian"
 * \param bReserve - if mdz_true reserve capacity when there is not enough space for insertion, otherwise mdz_false
 * \param pAsyncData - pointer to shared async data for asynchronous call, or NULL if call should be synchronous
 * \return:
 * mdz_false - if pUtf32 == NULL
 * mdz_false - if bReserve == mdz_true and memory allocation failed (MDZ_ERROR_ALLOCATION)
 * mdz_false - if bReserve == mdz_true and there is not enough capacity for inserted data, but m_pData is attached using mdz_utf32_attachData() (MDZ_ERROR_ATTACHED)
 * mdz_false - if bReserve == mdz_false and there is not enough free Capacity in the string (MDZ_ERROR_CAPACITY)
 * mdz_false - if pItems contain invalid 2-byte character(s) (MDZ_ERROR_CONTENT), or invalid enEndianness (MDZ_ERROR_ENDIANNESS)
 * mdz_false - if m_pData and pItems overlap (MDZ_ERROR_OVERLAP)
 * mdz_true  - if pItems == NULL (MDZ_ERROR_ITEMS), or nCount == 0 (MDZ_ERROR_ZEROCOUNT), or nLeftPos > Length (MDZ_ERROR_BIGLEFT). No insertion is made
 * mdz_true  - insertion succeeded
 */
mdz_bool mdz_utf32_insertUtf16_async(struct mdz_Utf32* pUtf32, size_t nLeftPos, const uint16_t* pItems, size_t nCount, enum mdz_endianness enEndianness, mdz_bool bReserve, struct mdz_asyncData* pAsyncData);

/**
 * Synchronous version
 */
#define mdz_utf32_insertUtf16(pUtf32, nLeftPos, pItems, nCount, enEndianness, bReserve) mdz_utf32_insertUtf16_async(pUtf32, nLeftPos, pItems, nCount, enEndianness, bReserve, NULL)

/**
 * Insert UTF-16 string pUtf16Source in string. Characters are converted to UTF-32 characters before isertion. Size grows on added UTF-32 characters count. Length grows on symbols count.
 * String m_pData and pUtf16Source->m_pData should not overlap.
 * \param pUtf32 - pointer to string returned by mdz_utf32_create() or mdz_utf32_create_attached()
 * \param nLeftPos - 0-based position to insert in symbols. If nLeftPos == Length or -1, items are appended. nLeftPos > Length is not allowed
 * \param pUtf16Source - pointer to UTF-16 string to insert. String is returned by mdz_utf16_create() or mdz_utf16_create_attached()
 * \param bReserve - if mdz_true reserve capacity when there is not enough space for insertion, otherwise mdz_false
 * \param pAsyncData - pointer to shared async data for asynchronous call, or NULL if call should be synchronous
 * \return:
 * mdz_false - if pUtf32 == NULL
 * mdz_false - if bReserve == mdz_true and memory allocation failed (MDZ_ERROR_ALLOCATION)
 * mdz_false - if bReserve == mdz_true and there is not enough capacity for inserted data, but m_pData is attached using mdz_utf32_attachData() (MDZ_ERROR_ATTACHED)
 * mdz_false - if bReserve == mdz_false and there is not enough free Capacity in the string (MDZ_ERROR_CAPACITY)
 * mdz_false - if m_pData and pUtf16Source->m_pData overlap (MDZ_ERROR_OVERLAP)
 * mdz_true  - if pUtf16Source == NULL (MDZ_ERROR_SOURCE), or pUtf16Source.Size == 0 (MDZ_ERROR_ZEROCOUNT), or nLeftPos > Length (MDZ_ERROR_BIGLEFT). No insertion is made
 * mdz_true  - insertion succeeded
 */
mdz_bool mdz_utf32_insertUtf16_string_async(struct mdz_Utf32* pUtf32, size_t nLeftPos, const void* pUtf16Source, mdz_bool bReserve, struct mdz_asyncData* pAsyncData);

/**
 * Synchronous version
 */
#define mdz_utf32_insertUtf16_string(pUtf32, nLeftPos, pUtf16Source, bReserve) mdz_utf32_insertUtf16_string_async(pUtf32, nLeftPos, pUtf16Source, bReserve, NULL)

#ifdef __cplusplus
}
#endif

#endif
