#ifndef COMPAT_H
#define COMPAT_H

#ifdef __cplusplus
# define BEGIN_DECLS extern "C" {
# define END_DECLS }
#else
# define BEGIN_DECLS
# define END_DECLS
#endif

#endif /* COMPAT_H */

