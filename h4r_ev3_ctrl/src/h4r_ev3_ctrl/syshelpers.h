/*
 * This file (syshelpers.h) is part of h4r_ev3_ctrl.
 * Date: 22.11.2015
 *
 * Author: Christian Holl
 * http://github.com/Hacks4ROS
 *
 * h4r_ev3_ctrl is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * h4r_ev3_ctrl is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with h4r_ev3_ctrl.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#ifndef SYSHELPERS_H_
#define SYSHELPERS_H_


#include <string>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

#include "Ev3Strings.h"

/**
 * Check if path exists (file or folder possible)
 * @param path The path to a file or folder
 * @return True if it exists, false otherwise
 */
bool pathExists(const std::string &path);

/**
 * Write int value into a opened (sys) file.
 * @param fileptr The file pointer from fopen
 * @param value	The value
 * @return	true if everything was successful, false otherwise
 */
bool writeIntToSysFile(FILE *fileptr,int value);

/**
 * Read integer value from (sys) file
 * @param fileptr The file pointer from fopen
 * @param[out] value	The value in the file only valid if successful
 * @return	true if everything was successful, false otherwise
 */
bool readIntFromSysFile(FILE *fileptr, int &value);

/**
 * Write a String in a map to a (sys) file
 * @param fileptr The file pointer
 * @param strmap The map with the strings and the keys
 * @param key The key (enum value)
 * @return True if everything was successful, false otherwise
 */
template <typename T>
bool writeKeyToSysFile(FILE *fileptr,const std::map<T,string> &strmap, T key)
{
	   typename std::map<T,string>::const_iterator it=strmap.find(key);
	   std::string out=it->second;
	   out+='\n';

	   int wrote=fwrite(out.c_str(),1,out.size(),fileptr);

	   if(fflush(fileptr))
		   return false;
	   rewind(fileptr);

	   return wrote==out.size();
}

/**
 * Reads a sysfile and checks if the string exists in a map and returns the value
 * @param fileptr
 * @param strmap
 * @param value The value for the string
 * @return True if everything was ok, false otherwise
 */
template <typename T>
bool readKeyFromSysFile(FILE *fileptr,const std::map<string,T> &strmap, T &value)
{
	   fflush(fileptr);
	   rewind(fileptr);
	   int64_t value_out;

	   char buffer[256];
	   char *buf=&buffer[0];
	   char **bufptr=&buf;

	   bool negative;
	   ssize_t read;
	   size_t len=256;
	   int l=0;
       while ((read = getline(bufptr, &len, fileptr)) != -1) {
    	   if(l==0)
    	   {
    		   buffer[read-1]=0x00;//remove linefeed!
    		   typename std::map<string,T>::const_iterator it=strmap.find(string(buffer));
    		   if(it!=strmap.end())
    		   {
    			   value=it->second;
    		   }
    		   else
    		   {
    			   value=(T)-1;
    		   }
    	 	   return true;
    	   }
    	   else
    	   {
    		   return false;
    	   }
       }


	   fscanf(fileptr,"%[^\n]",buffer);
	   return true;
}

#endif /* SYSHELPERS_H_ */
