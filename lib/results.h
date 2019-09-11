/*
 * Copyright (C) 2019  Red Hat, Inc.
 * Author(s):  David Cantrell <dcantrell@redhat.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

/*
 * This header defines the results constants.
 */

#ifndef _LIBRPMINSPECT_RESULTS_H
#define _LIBRPMINSPECT_RESULTS_H

/*
 * Inspection headers
 */
#define HEADER_METADATA      "header-metadata"
#define HEADER_EMPTYRPM      "payload"
#define HEADER_LICENSE       "license"
#define HEADER_ELF           "elf-object-properties"
#define HEADER_MAN           "man-pages"
#define HEADER_XML           "xml-files"
#define HEADER_DESKTOP       "desktop-entry-files"
#define HEADER_DISTTAG       "dist-tag"
#define HEADER_SPECNAME      "spec-file-name"
#define HEADER_MODULARITY    "modularity"
#define HEADER_JAVABYTECODE  "java-bytecode"
#define HEADER_CHANGEDFILES  "changed-files"
#define HEADER_REMOVEDFILES  "removed-files"
#define HEADER_ADDEDFILES    "added-files"

/*
 * Inspection remedies
 */
/* metadata */
#define REMEDY_VENDOR        "Change the string specified on the 'Vendor:' line in the spec file."
#define REMEDY_BUILDHOST     "Make sure the SRPM is built on a host within the expected subdomain."
#define REMEDY_BADWORDS      "Unprofessional language as defined in the configuration file was found in the text shown.  Remove or change the offending words and rebuild."

/* emptyrpm */
#define REMEDY_EMPTYRPM      "Check to see if you eliminated a subpackage but still have the %%package and/or the %%files section for it."

/* license */
#define REMEDY_LICENSE       "The License tag must contain an approved license string as defined by the distribution (e.g., GPLv2+)."
#define REMEDY_LICENSEDB     "Make sure the licensedb setting in the rpminspect.conf file is set to a valid licensedb file.  This is also commonly due to a missing vendor specific rpminspect-data package on the system."

/* elf */
#define REMEDY_ELF_TEXTREL              "Ensure all object files are compiled with -fPIC"
#define REMEDY_ELF_EXECSTACK_MISSING    "Ensure that the package is being built with the correct compiler and compiler flags"
#define REMEDY_ELF_EXECSTACK_INVALID    "The data in an ELF file appears to be corrupt; ensure that packaged ELF files are not being truncated or incorrectly modified"
#define REMEDY_ELF_EXECSTACK_EXECUTABLE "An ELF stack is marked as executable. Ensure that no execstack options are being passed to the linker, and that no functions are defined on the stack."
#define REMEDY_ELF_GNU_RELRO            "Ensure executables are linked with with '-z relro -z now'"
#define REMEDY_ELF_FORTIFY_SOURCE       "Ensure all object files are compiled with '-O2 -D_FORTIFY_SOURCE=2', and that all appropriate headers are included (no implicit function declarations). Symbols may also appear as unfortified if the compiler is unable to determine the size of a buffer, which is not necessarily an error."
#define REMEDY_ELF_FPIC                 "Ensure all object files are compiled with -fPIC"

/* man */
#define REMEDY_MAN_ERRORS   "Correct the errors in the man page as reported by the libmandoc parser."
#define REMEDY_MAN_PATH     "Correct the installation path for the man page. Man pages must be installed in the directory beneath /usr/share/man that matches the section number of the page."

/* xml */
#define REMEDY_XML          "Correct the reported errors in the XML document"

/* desktop */
#define REMEDY_DESKTOP "Refer to the Desktop Entry Specification at https://standards.freedesktop.org/desktop-entry-spec/latest/ for help correcting the errors and warnings"

/* disttag */
#define REMEDY_DISTTAG "The Release: tag in the spec file must include a '%%{?dist}' string.  Please add this to the spec file per the distribution packaging guidelines."

/* specname */
#define REMEDY_SPECNAME "The spec file name does not match the expected NAME.spec format.  Rename the spec file to conform to this policy."

/* modularity */
#define REMEDY_MODULARITY "This package is part of a module but is missing the %{modularitylabel} header tag.  Add this as a %%define in the spec file and rebuild."

/* javabytecode */
#define REMEDY_JAVABYTECODE "The Java bytecode version for one or more class files in the build was not met for the product release.  Ensure you are using the correct JDK for the build."

/* changedfiles */
#define REMEDY_CHANGEDFILES "Unexpected file changes were found.  Verify these changes are correct.  If they are not, adjust the build to prevent file changes."

/* removedfiles */
#define REMEDY_REMOVEDFILES "Unexpected file removals were found.  Verify these changes are correct.  If they are not, adjust the build to prevent the file removals."

/* addedfiles */
#define REMEDY_ADDEDFILES "Unexpected file additions were found.  Verify these changes are correct.  If they are not, adjust the build to prevent the file additions."

#endif
