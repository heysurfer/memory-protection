#include "Include/MemoryProtect.h"


void string_Test()
{
	Protect<std::string> MemoryProtection;
	MemoryProtection.setValue("test_1", "Hello!");
	printf("test_1 set to %s\n", MemoryProtection.getValue("test_1").c_str());
}
void uint32_Test()
{
	Protect<uint32_t> MemoryProtection;
	MemoryProtection.setValue("test_0", 50);
	printf("test_0 set to %i\n", MemoryProtection.getValue("test_0"));
}

void whileTest()
{
	Protect<uint32_t> Player;
	Player.setValue("AMMO", 32325);
	while (true)
	{
		printf("Ammo %i\n", Player.getValue("AMMO"));
		Sleep(1);
	}
}


int main()
{
	uint32_Test();
	string_Test();
	system("pause");
	whileTest();
}

