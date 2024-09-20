**February 2024 NOTE:** This repo is obsolete. Please use [mdz_string] project/repo instead.

**NOTE:** All 0.x releases are kind of "alpha-versions" without expectations of API backward-compatibility.

## Table of Contents
[mdz_unicode Overview](#mdz_unicode-Overview)<br>
[mdz_unicode Advantages](#mdz_unicode-Advantages)<br>
[mdz_unicode Usage](#mdz_unicode-Usage)<br>
[mdz_wchar Overview](#mdz_wchar-Overview)<br>
[mdz_utf8 Overview](#mdz_utf8-Overview)<br>
[mdz_utf16 Overview](#mdz_utf16-Overview)<br>
[mdz_utf32 Overview](#mdz_utf32-Overview)<br>
[Licensing info](#Licensing-info)<br>
[Credits](#Credits)

## mdz_unicode Overview

Wiki: [mdz_unicode Wiki]<br>
file: *"mdz_unicode.h"*

Please take a look at *"mdz_unicode.h"* file or [mdz_unicode Wiki] site for detailed functions descriptions.

[mdz_unicode Wiki]: https://github.com/maxdz-gmbh/mdz_unicode/wiki/mdz_unicode-overview

[mdz_unicode] - is a very lightweight, versatile and speedy C  library for handling Unicode strings, developed by [maxdz Software GmbH].The library supports UTF8, UTF16, UTF32, wchar strings. Source code of library is highly-portable, conforms to ANSI C 89/90 Standard. Builds for Win32/Win64, Linux, FreeBSD, Android, macOS are available.

Only shared/dynamically loaded libraries (*.so* and *.dll* files with import libraries) are available for evaluation testing purposes. Static libraries are covered by our commercial licenses.

**Linux** binaries are built against Linux Kernel 2.6.18 - and thus should be compatible with Debian (from ver. 4.0), Ubuntu (from ver. 8.04), Fedora (from ver. 9)

**FreeBSD** binaries - may be used from FreeBSD ver. 7.0

**Win32** binaries are built using Visual Studio Platform Toolset "v90", thus are compatible with Windows versions from Windows 2000.<br>
**Win64** binaries are built using Visual Studio Platform Toolset "v100", thus are compatible with Windows versions from Windows XP.<br>

**Android** x86/armeabi-v7a binaries - may be used from Android API level 16 ("Jelly Bean" ver. 4.1.x)<br>
**Android** x86_64/arm64-v8a binaries - may be used from Android API level 21 ("Lollipop" ver. 5.0)

**macOS** binaries - x86_64, from *MacOS X v10.6.0*

[mdz_unicode]: https://github.com/maxdz-gmbh/mdz_unicode
[maxdz Software GmbH]: https://maxdz.com/
[mdz_string]: https://github.com/maxdz-gmbh/mdz_string

## mdz_unicode Advantages

**1. High portability:** the whole code conforms to ANSI C 89/90 Standard. Multithreading/asynchronous part is POSIX compatible (under UNIX/Linux).

**2. Little dependencies:** basically *mdz_unicode* functions are only dependend on standard C-library memory-management/access functions. Multithreading part is dependend on POSIX *pthreads* API (under UNIX/Linux) and old process control/synchronization API (from Windows 2000). It means you can use library in your code withouth any further dependencies except standard platform libraries/APIs.

**3. Extended error-checking:** all functions preserve internal error-code pointing the problem. It is possible to use strict error-checking (when all preserved error-codes should be *MDZ_ERROR_NONE*) or "relaxed"-checking - when only returned *mdz_false* will indicate error.

**4. Extended control:** containers do only explicit operations. It means for example, when "insert" function is called with auto-reservation flag set in *mdz_false* - it will return error if there is not enough capacity in container. No implicit reservations will be made.

**5. Attached usage:** strings should not necessarily use dynamically-allocated memory - which may be not available on your embedded system (or if malloc()/free() are forbidden to use in your safety-critical software). Just attach container/data to your statically-allocated memory and use all strings functionality.

**6. Cache-friendly:** it is possible to keep controlling and data parts together in memory using "embedded part".

**7. Unicode support:** UTF-8, UTF-16, UTF-32 are supported.

**8. wchar_t support:** also wchar_t strings are supported, with 2 and 4 bytes-large *wchar_t* characters.

**9. Endianness-aware containers:** utf16 and utf32 containers are endiannes-aware thus may be used to produce and manipulate strings with pre-defined endianness even if endianness of host differs.

**10. Unicode "surrogate-pairs" awareness:** 2-byte Unicode strings correctly process/distinguish "surrogate-pairs" as 1 Unicode symbol.

**11. Asynchronous execution:** *insert* functions can be executed asynchronously

## mdz_unicode Usage

**unicode** is implemented with strict input parameters checking. It means *mdz_false* or some other error indication will be returned if one or several input parameters are invalid - even if such an invalidity doesn't lead to inconsistence (for example adding or removing 0 items).<br>

**Test license generation:** - in order to get free test-license, please proceed to our Shop page [maxdz Shop] and register an account. After registration you will be able to obtain free 14-days test-licenses for our products using "Obtain for free" button. 
Test license data should be used in *mdz_unicode_init()* call for library initialization.

**NOTE:** All 0.x releases are kind of "beta-versions" and can be used 1) only with test-license (during test period of 14 days, with necessity to re-generate license for the next 14 days test period) and 2) without expectations of interface backward-compatibility.

Several usage-scenarios are possible:
- low-level - raw C interface, using *mdz_unicode.h*, *mdz_utf8.h*, *mdz_utf16.h*, etc C-header files
- higher-level - using *MdzUnicode*, *MdzUtf8*, *MdzUtf16*, etc C++ "wrappers" around C-header files functions

[mdz_unicode Wiki]: https://github.com/maxdz-gmbh/mdz_unicode/wiki/mdz_unicode-overview
[maxdz Shop]: https://maxdz.com/shop.php

#### Code Example (low-level use)

*mdz_unicode_init()* with license information should be called for library initialization before any subsequent calls:

```
#include <mdz_unicode.h>

int main(int argc, char* argv[])
{
  /* mdz_unicode library initialization using test info retrieved after license generation (see "Test license generation" above) */
  
  mdz_bool bRet = mdz_unicode_init("<first-name-hash>", "<last-name-hash>", "<email-hash>", "<license-hash>");
  ...

  mdz_ansi_uninit(); /* call for un-initialization of library */
  
  return 0;
  
}
```

[mdz_utf8_create]: https://github.com/maxdz-gmbh/mdz_unicode/wiki/mdz_utf8_create
[mdz_utf8_destroy]: https://github.com/maxdz-gmbh/mdz_unicode/wiki/mdz_utf8_destroy

After library initialization call *[mdz_utf8_create]*() for **utf8** string creation. There should be also symmetric *[mdz_utf8_destroy]*() call for every create, otherwise allocated for string memory remains occupied:

```
#include <mdz_unicode.h>
#include <mdz_utf8.h>

int main(int argc, char* argv[])
{
  mdz_bool bRet = mdz_unicode_init("<first-name-hash>", "<last-name-hash>", "<email-hash>", "<license-hash>");
  
  // initialize pAnsi
  
  mdz_Utf8* pUtf8 = mdz_utf8_create(0); // create utf8-string
  ...
  ...
  // use pUtf8
  ...
  ...
  // destroy pUtf8
  
  mdz_utf8_destroy(&pUtf8); // after this pUtf8 should be NULL
  
  mdz_ansi_uninit();
  ...
}
```

Use *mdz_Utf8** pointer for subsequent library calls:

```
#include <mdz_unicode.h>
#include <mdz_utf8.h>

int main(int argc, char* argv[])
{
  mdz_bool bRet = mdz_unicode_init("<first-name-hash>", "<last-name-hash>", "<email-hash>", "<license-hash>");
  
  mdz_Utf8* pUtf8 = mdz_utf8_create(0); // create utf8-string

  // reserve memory for 5 elements
  
  bRet = mdz_utf8_reserve(pUtf8, 5);
  
  // insert 'b' in front position with auto-reservation if necessary
  
  bRet = mdz_utf8_insertAnsi(pUtf8, 0, "b", 1, mdz_true); // "b" after this call
  
  // append string with "cde" with auto-reservation if necessary
  
  bRet = mdz_utf8_insert(pUtf8, (size_t) -1, "cde", 3, mdz_true); // "bcde" after this call
  
  ...
  
  mdz_utf8_destroy(&pUtf8);
  
  mdz_ansi_uninit();
  ...
}
```

## mdz_wchar Overview
Wiki: [mdz_wchar Wiki]<br>
file: *"mdz_wchar.h"*

Please take a look at *"mdz_wchar.h"* file or [mdz_wchar Wiki] site for detailed functions descriptions.

[mdz_wchar Wiki]: https://github.com/maxdz-gmbh/mdz_containers/wiki/mdz_wchar-overview

## mdz_utf8 Overview
Wiki: [mdz_utf8 Wiki]<br>
file: *"mdz_utf8.h"*

Please take a look at *"mdz_utf8.h"* file or [mdz_utf8 Wiki] site for detailed functions descriptions.

[mdz_utf8 Wiki]: https://github.com/maxdz-gmbh/mdz_containers/wiki/mdz_utf8-overview

## mdz_utf16 Overview
Wiki: [mdz_utf16 Wiki]<br>
file: *"mdz_utf16.h"*

Please take a look at *"mdz_utf16.h"* file or [mdz_utf16 Wiki] site for detailed functions descriptions.

[mdz_utf16 Wiki]: https://github.com/maxdz-gmbh/mdz_containers/wiki/mdz_utf16-overview

## mdz_utf32 Overview
Wiki: [mdz_utf32 Wiki]<br>
file: *"mdz_utf32.h"*

Please take a look at *"mdz_utf32.h"* file or [mdz_utf32 Wiki] site for detailed functions descriptions.

[mdz_utf32 Wiki]: https://github.com/maxdz-gmbh/mdz_containers/wiki/mdz_utf32-overview

## Licensing info

Use of **mdz_unicode** library is regulated by license agreement in *LICENSE.txt*

Basically private non-commercial "test" usage is unrestricted. Commercial usage of library (incl. its source code) will be regulated by according license agreement.
