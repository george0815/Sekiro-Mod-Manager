# Sekiro-Mod-Manager
Mod Manger for the 2019 Game of The Year; Sekiro - Shadows Die Twice

The manager is extremely simple at the moment. Currently you can add, remove, install, and uninstall mods. Down the road I plan to add support for, launching sekiro, as well as automatically installing modengine(if it isnt installed already) and a UI overhaul.


This uses 7-Zip Extra(7za.exe) and UnRAR(UnRAR.exe) to unpack and repack mods, so when compiling the source code put 7za.exe(and its required dll files) and unrar.exe in the build directory.


This only supports mods that use modengine compressed in a RAR, Zip, or 7z format. It doesnt support dll chaining.



7-Zip Extra license

      7-Zip Extra
      ~~~~~~~~~~~
      License for use and distribution
      ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

      Copyright (C) 1999-2019 Igor Pavlov.

      7-Zip Extra files are under the GNU LGPL license.

      
      Notes: 
        You can use 7-Zip Extra on any computer, including a computer in a commercial 
        organization. You don't need to register or pay for 7-Zip.


      GNU LGPL information
      --------------------

        This library is free software; you can redistribute it and/or
        modify it under the terms of the GNU Lesser General Public
        License as published by the Free Software Foundation; either
        version 2.1 of the License, or (at your option) any later version.

        This library is distributed in the hope that it will be useful,
        but WITHOUT ANY WARRANTY; without even the implied warranty of
        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
        Lesser General Public License for more details.

        You can receive a copy of the GNU Lesser General Public License from 
        http://www.gnu.org/



UnRAR Licsense



      ******    *****   ******   UnRAR - free utility for RAR archives
      **   **  **   **  **   **  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
      ******   *******  ******    License for use and distribution of
      **   **  **   **  **   **   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
      **   **  **   **  **   **         FREEWARE version
                                   ~~~~~~~~~~~~~~~~

      The UnRAR utility is freeware. This means:

      1. All copyrights to RAR and the utility UnRAR are exclusively
      owned by the author - Alexander Roshal.

      2. The UnRAR utility may be freely distributed. It is allowed
      to distribute UnRAR inside of other software packages.

      3. THE RAR ARCHIVER AND THE UnRAR UTILITY ARE DISTRIBUTED "AS IS".
      NO WARRANTY OF ANY KIND IS EXPRESSED OR IMPLIED.  YOU USE AT 
      YOUR OWN RISK. THE AUTHOR WILL NOT BE LIABLE FOR DATA LOSS, 
      DAMAGES, LOSS OF PROFITS OR ANY OTHER KIND OF LOSS WHILE USING
      OR MISUSING THIS SOFTWARE.

      4. Neither RAR binary code, WinRAR binary code, UnRAR source or UnRAR
      binary code may be used or reverse engineered to re-create the RAR
      compression algorithm, which is proprietary, without written
      permission of the author.

      5. If you don't agree with terms of the license you must remove
      UnRAR files from your storage devices and cease to use the
      utility.

      Thank you for your interest in RAR and UnRAR.


                                            Alexander L. Roshal
