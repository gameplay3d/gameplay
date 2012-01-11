#include "gameplay.h"

void android_main(struct android_app* state)
{
	// Make sure glue isn't stripped.
    app_dummy();
	
	amain(state);
}
