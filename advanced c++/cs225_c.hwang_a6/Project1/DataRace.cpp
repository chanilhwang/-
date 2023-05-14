#include <map>
#include <string>
#include <iostream>
#include <windows.h>

class Account {
public:
    CRITICAL_SECTION cs_;
    /*_Guarded_by_(&cs_)*/ double balance;

	void withdraw(double amount)
	{
		if (balance < 0)
		{
			return;
		}
		EnterCriticalSection(&cs_);
		balance -= amount;
		LeaveCriticalSection(&cs_);
		return;
	}
};

