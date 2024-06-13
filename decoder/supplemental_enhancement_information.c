// #include <stdlib.h>
#include "supplemental_enhancement_information.h"
#include "common/misc.h"
#include "common/log.h"

void read_sei_rbsp(struct NAL_unit *nal)
{
}

void dump_sei_rbsp(FILE *fp, struct NAL_unit *nal)
{
    fprintf(fp, "    sei_rbsp() {\n");
    for (int i = 0; i < nal->rbsp.sei.count_messages; ++i)
    {
        fprintf(fp, "            sei_message() {\n");
        fprintf(fp, "                payloadType = 0\n");
        fprintf(fp, "                while(next_bits(8) == 0xFF) {\n");
        fprintf(fp, "                }\n");
        fprintf(fp, "            }\n");
    }
    fprintf(fp, "        rbsp_trailing_bits()\n");
    fprintf(fp, "    }\n");
}