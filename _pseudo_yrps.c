// should be generated YaRefPerSys/_pseudo_yrps.c
#include "yrps.h"
#ifndef YRPS_THIS_MODULE
#error YRPS_THIS_MODULE should be in the compilation command
#endif

void
_pseudo_inityrps (void)
{
  fprintf (stderr, "in %s:%d running %s (module %s id %s)\n", __FILE__,
	   __LINE__, __FUNCTION__, YRPS_THIS_MODULE, YRPS_ID);
  YRPS_UNIQUE_BREAKPOINT ();
#pragma message "compiling _pseudo_inityrps"
  fflush (NULL);
}


/// this is parsed in parse_generated_c_file_yrps
struct yrps_intvec_st yrps_v10 = {
  .vkind = Kyrps_intvect,
  .vlen = 3,
  .v_intvec = {-1, 2, 3}
};


struct yrps_object_st yrps_ob22637 = {
  .vkind = Kyrps_object,
  .vflag = 0,
  .o_id = 22637,
  .o_nbpair = 0,
  .o_nbval = 0,
  .o_flags = 0,
  .o_state = 0,
  .o_funad = NULL,
  .o_pairv = NULL,
  .o_valseq = NULL,
};
