#include "shm.hpp"
#include <unistd.h>
#include "Fifo.hpp"

int main()
{

    Shm shm(pathname, projid, CREATER);
    char *mem = (char *)shm.Virtualaddr();

    NamedFifo fifo(PATH, FILENAME);
    FileOper readerfile(PATH, FILENAME);
    readerfile.OpenForRead();
    while (true)
    {
        if (readerfile.Wait())
        {
            printf("%s\n", mem);
        }
        else
        {
            break;
        }
    }
    return 0;
}