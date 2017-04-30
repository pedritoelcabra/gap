/// PPP - Log


#ifndef _CLOG_H
    #define _CLOG_H

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <stdlib.h>

class CLog{
public:

    static const bool indb = false;

    static void  Write(const char* messText){
        try
          {
            if(getFileSize() > 200000){
                throw getFileSize();
            }
          }
          catch (long long e)
          {
            std::cout << "An exception occurred. File Size for Log above " << e << '\n';
          }
          FILE* pFile = fopen("log.txt", "a");
          fprintf(pFile, "%s\n", messText);
          fclose(pFile);
    }

    static void  Write(std::string messText){
        const char* messChar = messText.c_str();
        Write(messChar);
    }

    static void  Write(int number){
        Write(std::to_string(number));
    }

    static void  Write(double number){
        std::ostringstream strs;
        strs << number;
        Write(strs.str());
    }

    static void  Init(){
        remove("log.txt");
    }

    static long long getFileSize()
    {
        std::streampos fsize = 0;

        std::ifstream myfile ("log.txt", std::ios::in);  // File is of type const char*

        fsize = myfile.tellg();         // The file pointer is currently at the beginning
        myfile.seekg(0, std::ios::end);      // Place the file pointer at the end of file

        fsize = myfile.tellg() - fsize;
        myfile.close();

        static_assert(sizeof(fsize) >= sizeof(long long), "Oops.");

        return fsize;
    }

};


#endif
