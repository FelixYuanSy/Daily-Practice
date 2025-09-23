#include "shm.hpp"
#include <unistd.h>
#include "Fifo.hpp"
int main()
{
    FileOper writefile(PATH, FILENAME);
    writefile.OpenForWrite();

    Shm shm(pathname, projid, USER);
    char *mem = (char *)shm.Virtualaddr();
    int index = 0;
    for (char c = 'A'; c <= 'Z'; c++, index += 2)
    {
        sleep(2);
        mem[index] = c;
        mem[index + 1] = c;

        writefile.Wakeup();
    }
    writefile.Close();
    return 0;
}