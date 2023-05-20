#pragma once

void StaticAssertTest()
{
	//Example
	static_assert(sizeof(void*) == 4, "Requires 32-bit compilation.");

}