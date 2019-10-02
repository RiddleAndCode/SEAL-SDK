#include <sec_elem_abstr.h>
#include <stdio.h>
int main()
{
SE_STATUS result = SE_UNKNOWN;
do
{
    result = se_init(0);

} while (result != SE_SUCCESS);

printf("\n\t\t... SECURE ELEMENT has been initialized succesfully ...\n\n");
uint8_t rnd[32]= {0};
if(se_get_random(rnd,32) == SE_SUCCESS)
for (size_t i = 0; i < 32; i++)
{
printf("%02x ",rnd[i]);
}
puts("");







    return 0;
}
