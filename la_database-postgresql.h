#ifndef LA_DATABASE_POSTGRESQL_H
#define LA_DATABASE_POSTGRESQL_H

#include "la_database.h"

#ifdef __cplusplus
extern "C" {
#endif

void database_setSchema(LA_DATABASE *self, const char *schema);

#ifdef __cplusplus
}
#endif

#endif
