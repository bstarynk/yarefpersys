// should be generated YaRefPerSys/_pseudo_yrps.c
#include "yrps.h"
#ifndef YRPS_THIS_MODULE
#error YRPS_THIS_MODULE should be in the compilation command
#endif
void
_pseudo_inityrps(void)
{
  fprintf(stderr, "in %s:%d running %s (module %s id %s)\n", __FILE__, __LINE__,
	  __FUNCTION__, YRPS_THIS_MODULE, YRPS_ID);
#pragma message "compiling _pseudo_inityrps"
  fflush(NULL);
}
struct yrps_intvec_st yrps_v10 =
  {
    .vkind = Kyrps_intvect,
    .vlen = 3,
    .v_intvec = { -1, 2, 3}
  };
