#ifndef FT_NM_H
# define FT_NM_H

#include "loader.h"
#include "nlist.h"
#include "fat.h"

# define PROT		PROT_READ | PROT_WRITE
# define MAP		MAP_PRIVATE

# define X64	16
# define X86	8

# define LITTLE_ENDIAN	1
# define BIG_ENDIAN		0

# define BASE16		"0123456789abcdef"

# define NLIST64	((t_nlist64 *)d->array)
# define NLIST32	((t_nlist32 *)d->array)

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

typedef struct		s_data
{
	int			is_64;
	char		*ptr;
	uint32_t	ncmds;
	t_lc		*lc;
	t_symtab	*sym;
	void		*array;
	char		*string_table;
	uint32_t	n_value32;
	uint64_t	n_value64;
	uint8_t		n_type;
	uint8_t		n_sect;
}					t_data;

/*
**	get_type.c
*/
char				get_type(t_data *d, uint32_t i);

/*
**	sort_nlist.c
*/
void				sort_nlist(t_data *d);

/*
**	handle_32_64.c
*/
void				handle_32_64(char *ptr, int is_64);

/*
**	handle_fat.c
*/
void				handle_fat(char *ptr, int is_little_endian);

/*
**	print.c
*/
void				print_32_64(t_data *d, uint32_t i);


#endif
