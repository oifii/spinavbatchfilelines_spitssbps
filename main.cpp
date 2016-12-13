/*
 * Copyright (c) 2010-2016 Stephane Poirier
 *
 * stephane.poirier@oifii.org
 *
 * Stephane Poirier
 * 3532 rue Ste-Famille, #3
 * Montreal, QC, H2X 2L1
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#include <string>
#include <fstream>
#include <vector>

#include <iostream>
#include <sstream>
using namespace std;
//#include <stdlib.h>
#include <Windows.h>

#include <ctime>

int RandomInt(int lowest, int highest)
{
	int range = (highest - lowest) + 1;
	int random_integer = lowest + int(range*rand() / (RAND_MAX + 1.0));
	return random_integer;
}


int main(int argc, char *argv[])
{
	ofstream ofs("spinavbatchfilelines.txt");

	bool debug=false;
	//bool debug=true;

	///////////////////
	//read in arguments
	///////////////////
	float duration_s = 30.0f;
	string randomorsequential="random";
	string filterstring=""; //"" for no filter

	if(argc>1)
	{
		duration_s = atof(argv[1]);
	}
	if(argc>2)
	{
		randomorsequential = argv[2];
	}
	if(argc>3)
	{
		filterstring = argv[3];
	}

	if(debug==true)
	{
		duration_s = 0.001;
		randomorsequential="sequential";
	}

	//////////////////////////
	//initialize random number
	//////////////////////////
	srand((unsigned)time(0));

	//////////////////////////////////////////
	//load batch file lines into string vector
	//////////////////////////////////////////
	vector<string> batchfilelines;
	ifstream ifs("spitonicstepsequencerbufferplayersynthvoroguiwin32.bat");
	string temp;
	int ii=-1;
	while(getline(ifs,temp))
	{
		ii++;
		if(ii==0)
		{
			temp="taskkill /im spitonicstepsequencerbufferplayersynthvoroguiwin32.exe";
			batchfilelines.push_back(temp);
			continue;
		}
		if(!filterstring.empty())
		{
			string TEMP = "";
			for (std::string::size_type i=0; i<temp.length(); ++i)
			{
				TEMP+= toupper(temp[i]);
			}
			string FILTERSTRING = "";
			for (std::string::size_type i=0; i<filterstring.length(); ++i)
			{
				FILTERSTRING+= toupper(filterstring[i]);
			}
			size_t found = TEMP.find(FILTERSTRING, 64); //search from 65th char 
			if(found==string::npos) continue;
		}
		//remove "rem " from string if "rem " present
		size_t pos = temp.find("start");
		if(pos==4) temp = temp.substr(pos);
		if(pos==0 || pos==4)
		{
			if(!temp.empty()) batchfilelines.push_back(temp);
		}
	}

	////////////////////////////////////////////////////
	//browse throught batchfilelines and execute command
	////////////////////////////////////////////////////
	string quote;
	quote = "\"";
	string cmdbuffer;

	vector<string>::iterator it;
	for ( it=batchfilelines.begin() ; it < batchfilelines.end(); it++ )
	{
		if(randomorsequential=="random")
		{
			//taskkill
			cmdbuffer = batchfilelines[0];
			cout << cmdbuffer << endl;
			if(debug==false) system(cmdbuffer.c_str()); 
			//start ... command
			int idline = RandomInt(1, batchfilelines.size()-1);
			cmdbuffer = batchfilelines[idline];
			cout << cmdbuffer << endl;
			ofs << cmdbuffer << endl;
			ofs.flush();
			if(debug==false) system(cmdbuffer.c_str()); 
		}
		else
		{
			//taskkill
			cmdbuffer = batchfilelines[0];
			cout << cmdbuffer << endl;
			if(debug==false) system(cmdbuffer.c_str()); 
			//start ... command
			cmdbuffer = *it;
			cout << cmdbuffer << endl;
			ofs << cmdbuffer << endl;
			ofs.flush();
			if(debug==false) system(cmdbuffer.c_str()); 
		}
		Sleep((int)(duration_s*1000));
	}
	return 0;
}