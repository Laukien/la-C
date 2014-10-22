/*
 * =====================================================================================
 *
 *       Filename:  la_atomic.h
 *    Description:  synchronize code
 *         Author:  Stephan Laukien
 *          Email:  software@laukien.com
 *        License:  Simplified BSD License
 *      Copyright:  (c) 2011-2014
 *
 * =====================================================================================
 */

#ifndef LA_ATOMIC_H
#define LA_ATOMIC_H


#ifdef __cplusplus
#include <string>

extern "C" {
#endif

typedef struct la_atomic ATOMIC;

ATOMIC *atomic_new();
void atomic_free(ATOMIC *self);
void atomic_lock(ATOMIC *self);
void atomic_unlock(ATOMIC *self);

#ifdef __cplusplus
}

namespace la {
	namespace atomic {
		class Atomic {
			private:
				ATOMIC *obj;
			public:
				Atomic();
				~Atomic();
				void lock();
				void unlock();
		};
	}
}
#endif

#endif
