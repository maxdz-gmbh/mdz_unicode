/**
* \ingroup mdz_unicode library
*
* \author maxdz Software GmbH
*
* \par license
* This file is subject to the terms and conditions defined in file 'LICENSE.txt', which is part of this source code package.
*
* \par description
* mdz_unicode initialization functions.
*
* \par portability
* Source code of library conforms to ANSI C 89/90 Standard.
*
* \version 0.4
*
* \date 2021-04
*
*/

#ifndef MDZ_UNICODE_H
#define MDZ_UNICODE_H

#include "mdz_types.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * \defgroup Init functions
 */

/**
 * Initializes unicode library. This function should be called before any other function of the library.
 * \param pFirstNameHash - user first name hash code
 * \param pLastNameHash - user last name hash code
 * \param pEmailHash - user e-mail hash code
 * \param pLicenseHash - license hash code
 * \return:
 * mdz_true - if the initialization has succeed, otherwise false
 */
mdz_bool mdz_unicode_init(const uint32_t* pFirstNameHash, const uint32_t* pLastNameHash, const uint32_t* pEmailHash, const uint32_t* pLicenseHash);

/**
 * Initializes unicode library. This function should be caled before any other function of the library.
 * Memory for license data starts at position pStart. Size of internal initialization structure is returned in pSize.
 * \param pFirstNameHash - user first name hash code
 * \param pLastNameHash - user last name hash code
 * \param pEmailHash - user e-mail hash code
 * \param pLicenseHash - license hash code
 * \param pStart - memory start position of license data
 * \param nAreaSize - size of available memory from pStart in bytes. Should be large enough for license data (> 500 bytes)
 * \param pOutSize - actual size of placed license data in bytes
 * \return:
 * mdz_true - if the initialization has succeed, otherwise false
 */
mdz_bool mdz_unicode_init_attached(const uint32_t* pFirstNameHash, const uint32_t* pLastNameHash, const uint32_t* pEmailHash, const uint32_t* pLicenseHash, const char* pStart, size_t nAreaSize, size_t* pOutSize);

/**
 * Un-initializes unicode library and frees corresponding memory allocations.
 */
void mdz_unicode_uninit(void);

#ifdef __cplusplus
}
#endif

#endif
