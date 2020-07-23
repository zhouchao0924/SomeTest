
#pragma once

// These are the result codes:
#define SIMPLE_ZIP_OK         0x00000000     // nb. the pseudo-code zr-recent is never returned,
#define SIMPLE_ZIP_RECENT     0x00000001     // but can be passed to FormatZipMessage.
// The following come from general system stuff (e.g. files not openable)
#define SIMPLE_ZIP_GENMASK    0x0000FF00
#define SIMPLE_ZIP_NODUPH     0x00000100     // couldn't duplicate the handle
#define SIMPLE_ZIP_NOFILE     0x00000200     // couldn't create/open the file
#define SIMPLE_ZIP_NOALLOC    0x00000300     // failed to allocate some resource
#define SIMPLE_ZIP_WRITE      0x00000400     // a general error writing to the file
#define SIMPLE_ZIP_NOTFOUND   0x00000500     // couldn't find that file in the zip
#define SIMPLE_ZIP_MORE       0x00000600     // there's still more data to be unzipped
#define SIMPLE_ZIP_CORRUPT    0x00000700     // the zipfile is corrupt or not a zipfile
#define SIMPLE_ZIP_READ       0x00000800     // a general error reading the file
// The following come from mistakes on the part of the caller
#define SIMPLE_ZIP_CALLERMASK 0x00FF0000
#define SIMPLE_ZIP_ARGS       0x00010000     // general mistake with the arguments
#define SIMPLE_ZIP_NOTMMAP    0x00020000     // tried to ZipGetMemory, but that only works on mmap zipfiles, which yours wasn't
#define SIMPLE_ZIP_MEMSIZE    0x00030000     // the memory size is too small
#define SIMPLE_ZIP_FAILED     0x00040000     // the thing was already failed when you called this function
#define SIMPLE_ZIP_ENDED      0x00050000     // the zip creation has already been closed
#define SIMPLE_ZIP_MISSIZE    0x00060000     // the indicated input file size turned out mistaken
#define SIMPLE_ZIP_PARTIALUNZ 0x00070000     // the file had already been partially unzipped
#define SIMPLE_ZIP_ZMODE      0x00080000     // tried to mix creating/opening a zip 
// The following come from bugs within the zip library itself
#define SIMPLE_ZIP_BUGMASK    0xFF000000
#define SIMPLE_ZIP_NOTINITED  0x01000000     // initialisation didn't work
#define SIMPLE_ZIP_SEEK       0x02000000     // trying to seek in an unseekable file
#define SIMPLE_ZIP_NOCHANGE   0x04000000     // changed its mind on storage, but not allowed
#define SIMPLE_ZIP_FLATE      0x05000000     // an internal error in the de/inflation code

namespace SimpleZIP
{
	void CollectErrorInformation(unsigned long Code);
}