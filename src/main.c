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








    return 0;
}
