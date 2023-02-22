### memory-protection

A library that provides memory protection by using the xxHash algorithm to hash and encrypt protected values stored in memory. The algorithm makes it difficult for cheating tools like cheat engines to access or modify the protected values.


Example Usage :
```c++
Protect<std::string> MemoryProtection;
MemoryProtection.setValue("test_1", "Hello!");
printf("test_1 set to %s\n", MemoryProtection.getValue("test_1").c_str());
```
```c++
Protect<uint32_t> MemoryProtection;
MemoryProtection.setValue("test_0", 50);
printf("test_0 set to %i\n", MemoryProtection.getValue("test_0"));
```



https://user-images.githubusercontent.com/58826689/220785503-a4b6847a-31a9-402e-9165-1312ec9b4ef4.mp4

