#include <iostream>
#include <stdio.h>
#include <intrin.h>

#include "x64ret_addr_spoof.hpp"

#pragma section(".text")
__declspec(allocate(".text")) const unsigned char jmp_rdx[] = { 0xFF, 0x27 };

void sozgen() {
	std::cout << "return address: " << _ReturnAddress() << std::endl; // print address of jmp_rdx
}

int main() {
	std::cout << "main: " << std::hex << (void*)main << std::endl;
	std::cout << "jmp_rdx: " << (void*)jmp_rdx << std::endl;
	return_spoofing::spoofing_call(jmp_rdx, &sozgen);

	system("pause");
	return 0;
}