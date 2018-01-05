#ifndef FT_NM_H
# define FT_NM_H

#include "loader.h"
#include "nlist.h"

# define PROT		PROT_READ | PROT_WRITE
# define MAP		MAP_PRIVATE

# define X64	1
# define X86	0

# define NLIST64	((t_nlist64 *)array)
# define NLIST32	((t_nlist32 *)array)

typedef struct load_command			t_lc;
typedef struct symtab_command		t_symtab;
typedef struct mach_header_64		t_header64;
typedef struct mach_header			t_header32;
typedef struct fat_header			t_headerfat;
typedef struct fat_arch				t_arch;
typedef struct nlist_64				t_nlist64;
typedef struct nlist				t_nlist32;
typedef struct segment_command_64	t_seg64;
typedef struct segment_command		t_seg32;
typedef struct section_64			t_sect64;
typedef struct section				t_sect32;

typedef struct		s_ndata
{
	t_lc		*lc;
	uint32_t	ncmds;
	uint32_t	n_value;
	uint8_t		n_type;
	uint8_t		n_sect;
}					t_ndata;

/*
**	get_type.c
*/
char				get_type(void *array, uint32_t i, void *ptr, int is_64);

/*
**	sort_nlist.c
*/
void				sort_nlist(void *array, char *string_table,
								uint32_t nsyms, int is_64);

/*
**	handle_32_64.c
*/
void				handle_32_64(char *ptr, int is_64);
#endif
