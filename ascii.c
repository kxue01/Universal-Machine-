#include <stdio.h>
#include <stdlib.h>

int main()
{
        FILE *fp = fopen("ascii.txt", "w");
        for (int i = 0; i < 256; i++) {
                fputc(i, fp);
        }
        fclose(fp);
        return 0;
}
