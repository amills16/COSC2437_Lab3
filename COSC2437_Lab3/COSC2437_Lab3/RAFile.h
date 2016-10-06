#ifndef RAFILE_H
#define RAFILE_H

#include <fstream>
#include <vector>
#include "wcs_string.h"
#include "WCS_UB_Array.h"

using namespace std;


typedef	unsigned long	ULong;

class RAFile
{
	private:
		struct Recinfo
		{
			streamoff	Offset;
			ULong		Length;
		};
	public:
		enum	ErrCodes {OK, FileAlreadyOpen, CannotOpenFile, FileNotOpen, InvalidIndex};

							RAFile		();
							~RAFile		();
		ErrCodes			Close		();
		ErrCodes			Get			(ULong, WCS_String &);
		ErrCodes			Insert		(ULong, const WCS_String &);
		ErrCodes			Open		(const WCS_String &);
		ErrCodes			Replace		(ULong, const WCS_String &);
		void				Dump		();
	private:
							RAFile		(const RAFile &);
		RAFile &			operator =	(const RAFile &);

		ULong							NumRecords;
		streamoff						EndOfLastRecord;
		fstream							File;
		WCS_UB_Array <Recinfo, ULong>	RecinfoArray;
};

#endif
