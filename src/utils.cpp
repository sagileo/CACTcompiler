#include "utils.h"

double sci2db(const std::string &strSci)
{	
	int    iPower       = 0;  //幂
	double dMntsa       = 0;  //尾数
	double dCoefficient = 1;  //系数

	std::string strPower, strMntsa;

	if (std::string::npos == strSci.find("E"))
	{
		return atof(strSci.c_str());
	}

	strMntsa = strSci.substr(0, strSci.find("E"));
	strPower = strSci.substr(strSci.find("E") + 1);

	dMntsa = atof(strMntsa.c_str());
	iPower = atoi(strPower.c_str());

	while (iPower != 0)
	{
		if (iPower > 0)
		{
			dCoefficient *= 10;
			iPower--;
    	}
		else
		{
			dCoefficient *= 0.1;
			iPower++;
		}
  	}

  	return dMntsa * dCoefficient;
}
