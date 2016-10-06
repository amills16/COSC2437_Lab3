#include "RAFile.h"

RAFile::RAFile ()
{
}

RAFile::~RAFile  ()
{
	if (File.is_open ())
		Close ();
	else;
}

RAFile::ErrCodes RAFile::Close ()
	{
	ULong		i;

	File.seekp (0);
	File.write ((const char *) &NumRecords, sizeof (NumRecords));
	File.seekp (EndOfLastRecord);
	for (i = 0; i < NumRecords; i++)
		File.write ((const char *) &RecinfoArray [i], sizeof (Recinfo));
	File.close ();
	return OK;
	}

RAFile::ErrCodes RAFile::Get (ULong Index, WCS_String & Str)
{
	char *	pChars;

	if (File.is_open ())
		if (Index < NumRecords)
		{
			if (RecinfoArray [Index].Length == 0)
			{
				Str = "";	// empty string
				return OK;
			}
			else
			{
				File.seekg (RecinfoArray [Index].Offset);
				pChars = new char [RecinfoArray [Index].Length + 1];
				File.read (pChars, RecinfoArray [Index].Length);
				pChars [RecinfoArray [Index].Length] = '\0';
				Str = pChars;
				delete [] pChars;
				return OK;
			}
		}
		else
			return InvalidIndex;
	else
		return FileNotOpen;
}

RAFile::ErrCodes RAFile::Insert (ULong Index, const WCS_String & Str)
{
	int		i;
	if (File.is_open())
	{
		if (Index >= NumRecords)
		{
			Replace(Index, Str);
		}
		else
		{
			for (i = NumRecords; i >= Index && i >= 0; i--)
			{
				RecinfoArray[i+1] = RecinfoArray[i];
			}
			RecinfoArray[Index].Offset = EndOfLastRecord;
			RecinfoArray[Index].Length = Str.Length();
			File.seekp(EndOfLastRecord);
			File.write((const char *)Str, Str.Length());
			EndOfLastRecord = File.tellp();
			NumRecords++;
			return OK;
		}
		return OK;
	}
	else
	{
		return FileNotOpen;
	}
}

RAFile::ErrCodes RAFile::Open (const WCS_String & FileName)
	{
	ULong		i;

	if (File.is_open ())
			return FileAlreadyOpen;
	else
	{
		File.open ((const char *) FileName, ios_base::in | ios_base::out | ios_base::binary);
		if (!File.is_open ())
		{
			File.clear ();	// must clear error flags before I can do anything else
			File.open ((const char *) FileName, ios_base::out | ios_base::binary);
			if (!File.is_open ())
				return CannotOpenFile;
			else
			{
				NumRecords	= 0;
				File.write ((const char *) &NumRecords, sizeof (NumRecords));
				File.close ();
				File.open ((const char *) FileName, ios_base::in | ios_base::out | ios_base::binary);
				if (!File.is_open ())
					return CannotOpenFile;
				else;
			}
		}
		else;
		// at this point the file is open
		File.read ((char *) &NumRecords, sizeof (NumRecords));
		File.seekg (-(long long) (sizeof (Recinfo) * NumRecords), ios_base::end);	// go to end of file and back up to where the record info begins
		EndOfLastRecord	= File.tellg ();	// tells me the current offset from the beginning of the file
		for (i = 0; i < NumRecords; i++)
			File.read ((char *) &RecinfoArray [i], sizeof (Recinfo));
		return OK;
	}
}

RAFile::ErrCodes RAFile::Replace (ULong Index, const WCS_String & Str)
{
	ULong		i;
	if (File.is_open ())
	{
		if (Index >= NumRecords)
		{
			for (i = NumRecords; i < Index; i++)
			{
				RecinfoArray [i].Offset = 0;
				RecinfoArray [i].Length	= 0;
				NumRecords++;
			}
			RecinfoArray [Index].Offset = EndOfLastRecord;
			RecinfoArray [Index].Length	= Str.Length ();
			File.seekp (EndOfLastRecord);
			File.write ((const char *) Str, Str.Length ());
			EndOfLastRecord				= File.tellp ();
			NumRecords++;
			return OK;
		}
		else
			if (Str.Length () <= RecinfoArray [Index].Length)	// is there enough space in the current record
			{
				File.seekp (RecinfoArray [Index].Offset);	// go to where the current record is
				File.write ((const char *) Str, Str.Length ());
				RecinfoArray [Index].Length = Str.Length ();	// set new length of record in file
			}
			else
			{
				File.seekp (EndOfLastRecord);	// move to end of file since that is where we have space to write
				RecinfoArray [Index].Offset = EndOfLastRecord;
				RecinfoArray [Index].Length = Str.Length ();	// set new length of record in file
				File.write ((const char *) Str, Str.Length ());
				EndOfLastRecord				= File.tellp ();	// set new position marking the end of the last positioned record in the file
			}
		return OK;
	}
	else
	{
		return FileNotOpen;
	}
}

void RAFile::Dump()
{
	WCS_String Data;
	for (int i = 0; i < NumRecords; i++)
	{
		Get(i, Data);
		cout << "File at index: " << i << " is " <<  Data << endl;
	}
}
