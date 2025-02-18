
#include <fstream>
#include <limits>

using namespace std;

int main()
{
    ifstream file("./ReadFileToBuffer.cpp", ios::binary | ios::ate);
    streamsize size = file.tellg();
    
    if (size >= numeric_limits<size_t>::max())
    {
		printf("File is to big\r\n");
		return 1;
	}
	size_t bufferSize = (size_t)size;
	
    file.seekg(0, ios::beg);

    char* buffer = new char[bufferSize+1];
    buffer[bufferSize] = 0;

    if (file.read(buffer, bufferSize))
    {
		printf("%s\r\n", buffer);
    } else
    {
		printf("Cannot read file\r\n");
	}

    free(buffer);

    return 0;
}


