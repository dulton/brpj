#ifndef _CURL_BASE2_H_
#define _CURL_BASE2_H_
#include "typedefine.h"
using namespace std;

void File2String(char *Path,string &Data);

int	JingAoUpload(char *I_IpPort,char *I_url,char *I_Part1,char *I_Part2,char *I_Part3,
				 char *I_smallfilename,char *I_smallfilepath,
				 char *I_filename,char *I_filepath,char *jsonstr);


#endif
