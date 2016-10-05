#include <iostream>
#include <stdlib.h>
#include <conio.h>
#include "RAFile.h"

using namespace std;

int ReadInt();
void main()
{
	int			pos;
	char		c;
	WCS_String	FileName("Test.dat");
	WCS_String	commands;
	RAFile		DB1;

	DB1.Open(FileName);
	FileName = "XXXXX";
	DB1.Replace(0, FileName);
	FileName = "YYYY";
	DB1.Replace(1, FileName);
	FileName = "ZZZ";
	DB1.Replace(5, FileName);
	FileName = "AAAA";
	DB1.Insert(0, FileName);
	DB1.Get(0, FileName);
	cout << "Data read was: " << FileName << endl;
	DB1.Get(1, FileName);
	cout << "Data read was: " << FileName << endl;
	DB1.Get(2, FileName);
	cout << "Data read was: " << FileName << endl;
	DB1.Get(3, FileName);
	cout << "Data read was: " << FileName << endl;
	DB1.Get(4, FileName);
	cout << "Data read was: " << FileName << endl;
	DB1.Get(5, FileName);
	cout << "Data read was: " << FileName << endl;
	DB1.Get(6, FileName);
	cout << "Data read was: " << FileName << endl;
	cout << "What operation would you like to perform? (R,I,G,x): ";
	commands.Read();
	commands.GetAt(c, 0);
	cout << "Input database location: ";
	pos = ReadInt();
	while ((c != 'x'))
	{
		switch (c)
		{
		case 'R':
		case 'r':
			pos = ReadInt();
			cout << "Input filename here: ";
			FileName.Read();
			DB1.Replace(pos, FileName);
			break;

		case 'I':
		case 'i':
			pos = ReadInt();
			cout << "Input filename here: ";
			FileName.Read();
			DB1.Insert(pos, FileName);
			break;

		case 'G':
		case 'g':
			pos = ReadInt();
			DB1.Get(pos, FileName);
			cout << FileName << endl;
			break;

		default:
			cout << "Please enter a valid command... \n";
			break;
		}
		cout << "What operation would you like to perform? (R,I,G,x): ";
		commands.Read();
		commands.GetAt(c, 0);
	}
	DB1.Close ();
}

int ReadInt()
{
	char	c;
	bool	Negative;
	int		NumKeysHit;
	int		Num;

	Negative = false;
	Num = 0;
	NumKeysHit = 0;

	cout << "Input database location: ";
	while ((c = _getch()) != '\r')
	{
		switch (c)
		{
		case '-':
			if (NumKeysHit == 0)
			{
				Negative = true;
				NumKeysHit++;
			}
			else
			{
				c = '\a';
			}
			break;
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			Num = (Num * 10) + (c - '0');
			NumKeysHit++;
			break;
		case '\b':
			if (NumKeysHit > 0)
			{
				Num = Num / 10;
				_putch('\b');
				_putch(' ');
				NumKeysHit--;
				if (NumKeysHit == 0)	Negative = false;
			}
			else
			{
				c = '\a';
			}
			break;
		default:
			c = '\a';
		}
		_putch(c);
	}
	if (Negative)	Num = -Num;
	_putch('\n');
	return Num;
}