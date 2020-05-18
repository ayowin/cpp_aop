
#include <iostream>
#include <functional>

#include "GeneralAop.hpp"

void aopTest1(int data)
{
	printf("test1...%d\n",data);
}

class AopTest2
{
public:
	void test(int data1,std::string data2)
	{
		printf("test2...%d,%s\n",data1,data2.c_str());
	}
};

class Log
{
public:
	void before(int data)
	{
		printf("log before...%d\n",data);
	}
	void after(int data)
	{
		printf("log after...%d\n",data);
	}

	void before(int data1,std::string data2)
	{
		printf("log before...%d,%s\n",data1,data2.c_str());
	}
	void after(int data1,std::string data2)
	{
		printf("log after...%d,%s\n",data1,data2.c_str());
	}
};

class Security
{
public:
	void before(int data)
	{
		printf("security before...%d\n",data);
	}
	void after(int data)
	{
		printf("security after...%d\n",data);
	}

	void before(int data1,std::string data2)
	{
		printf("security before...%d,%s\n",data1,data2.c_str());
	}
	void after(int data1,std::string data2)
	{
		printf("security after...%d,%s\n",data1,data2.c_str());
	}
};

int main()
{
	/* test1 */
	GeneralAop::invoke<Log,Security>(&aopTest1, 123456);

	printf("------------------------------------------------\n");

	/* test2 */
	AopTest2 test2;
	GeneralAop::invoke<Log,Security>(std::bind(&AopTest2::test,&test2,std::placeholders::_1,std::placeholders::_2),654321,"blablabla");
	
	return 0;
}