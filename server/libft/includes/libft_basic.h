/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft_basic.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clala <clala@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/14 13:22:49 by clala             #+#    #+#             */
/*   Updated: 2021/04/16 22:34:37 by clala            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_BASIC_H
# define LIBFT_BASIC_H

# include <string.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdint.h>
# include <stdarg.h>
# include "error.h"

# define HEX_CHARS "0123456789ABCDEF"

typedef struct s_list
{
	void				*content;
	size_t				content_size;
	struct s_list		*next;
}	t_list;

typedef struct s_point
{
	int					x;
	int					y;
}	t_point;

/*
**	2dchararr_terminated
*/
int						free_2dchararr_terminated(char **arr);
int						len_2dchararr_terminated(char **arr);
int						print_2dchararr_terminated(char **arr);
char					**ft_copy2dchararr(char **src);
/*
**	arrays
*/
char					**ft_create2dchararr(int rows, int cols);
int						*ft_intarrnew(int size);
int						**ft_intmatrixnew(int rows, int cols);
char					**ft_strarrnew(int x, int y);

/*
**	error
*/
int						handle_error(char *s);

/*
**	bint
*/
char					*ft_bintaddtn(char *operand_1, char *operand_2);
int						ft_bintcmp(char *operand_1, char *operand_2);
char					*ft_bintsubtrn(char *operand_1, char *operand_2);
char					*ft_bintfmt(char *number);
char					*ft_bintroundfree(char *num, int precision,
							int free_num);
char					*ft_bintmltpl(char *num, int multiplier);
char					*ft_bintdivsn(char *num, int divider);
void					ft_bintprepr(char **op_1, char **op_2);

/*
**	conversion
*/
int						ft_abs(int n);
int						ft_ctoi(char c);
int						ft_itoc(int n);
char					*ft_itoa_base(int value, int base);
char					*ft_uiptrtoa_base(uintptr_t value, int base);
char					*ft_dtoa(double n, int precision);
char					*ft_ldtoa(long double n, int precision);
char					*ft_utoa_base(uintmax_t n, int precision);
char					*ft_ltoa_base(long long value, int base);
char					*ft_ultoa_base(unsigned long long value, int base);
char					*ft_uitoa_base(unsigned int value, int base);
int						ft_atoi (const char *s);
int						ft_atoi_base(char *str, int base);
char					*ft_itoa(int n);

/*
**	ctype
*/
int						ft_isalnum(int c);
int						ft_isalpha(int c);
int						ft_isascii(int c);
int						ft_isdigit(int c);
int						ft_isprint(int c);
int						ft_isinteger(char *s);

/*
**	list
*/
void					ft_lstadd(t_list **alst, t_list *new);
void					ft_lstdel(t_list **alst, void (*del)(void *, size_t));
void					ft_lstdelone(t_list **alst,
							void (*del)(void *, size_t));
void					ft_lstiter(t_list *lst, void (*f)(t_list *elem));
t_list					*ft_lstmap(t_list *lst, t_list *(*f)(t_list *elem));
t_list					*ft_lstnew(void const *content, size_t content_size);
t_list					*ft_listpop(t_list *list, void *value);
t_list					*ft_lstnew_pointer(void const *content, size_t content_size);

/*
**	math
*/
double					ft_pow(double x, double y);
int						ft_imaxval(char *format, ...);
int						ft_ilen(int n);
int						ft_imaxlen(intmax_t n);
size_t					ft_uimaxlen(uintmax_t n);
size_t					ft_uimaxtmaxlen(uintmax_t n);
int						ft_clamp(int d, int min, int max);

/*
**	memory
*/
void					*ft_memccpy(void *dest, const void *src,
							int c, size_t n);
void					*ft_memchr (const void *arr, int c, size_t n);
int						ft_memcmp(const void *ptr1, const void *ptr2,
							size_t num);
void					*ft_memcpy(void *dest, const void *src, size_t n);
void					*ft_memmove(void *dst, const void *src, size_t n);
void					*ft_memset(void *destination, int c, size_t n);
int						ft_free2dchararr(char **arr, int rows);
void					*ft_memalloc(size_t size);
void					ft_memdel(void **ap);
int						ft_free(char *format, ...);
void					ft_bzero(void *dest, size_t n);
int						ft_free_int(void *to_free);
int						ft_free_null(void **to_free);

/*
**	string
*/
char					*ft_strnlstrip(char const *s, char *charset, int n);
void					ft_strswap(char **a, char **b);
size_t					ft_strnlen(const char *s, size_t maxlen);
char					*ft_strndup(const char *src, size_t n);
char					*ft_strtrimend(char const *s, char *charset);
char					*ft_strjoinfree(char const *s1, char const *s2,
							int free_s1, int free_s2);
char					*ft_strnewchr(int size, char c);
char					*ft_strtrimstart(char const *s, char *charset);
void					ft_strclr(char *s);
void					ft_strdel(char **as);
int						ft_strequ(char const *s1, char const *s2);
void					ft_striter(char *s, void (*f)(char *));
void					ft_striteri(char *s, void (*f)(unsigned int, char *));
char					*ft_strjoin(char const *s1, char const *s2);
char					*ft_strmap(char const *s, char (*f)(char));
char					*ft_strmapi(char const *s,
							char (*f)(unsigned int, char));
int						ft_strnequ(char const *s1, char const *s2, size_t n);
char					*ft_strnew(size_t size);
char					**ft_strsplit(char const *s, char c);
char					*ft_strsub(char const *s,
							unsigned int start, size_t len);
char					*ft_strtrim(char const *s);
char					*ft_strcat (char *dest, const char *app);
char					*ft_strchr (const char *str, int ch);
int						ft_strcmp(const char *s1, const char *s2);
char					*ft_strcpy(char *dest, const char *src);
char					*ft_strdup(const char *src);
size_t					ft_strlcat (char *dest, const char *app, size_t n);
size_t					ft_strlen(const char *str);
char					*ft_strncat (char *dest, const char *app, size_t n);
int						ft_strncmp(const char *s1, const char *s2, size_t n);
char					*ft_strncpy(char *dest, const char *src, size_t n);
char					*ft_strnstr(const char *str, const char *substr, \
							size_t n);
char					*ft_strrchr (const char *str, int ch);
char					*ft_strstr(const char *str, const char *substr);
int						ft_tolower(int c);
int						ft_toupper(int c);
char					*ft_strlwr(char *s);
char					*ft_strupr(char *s);
void					ft_strrev(char *s);
char					*ft_strchrvar(const char *str, char *format, ...);
char					*ft_strchrset(const char *str, const char *charset);
char					*ft_strreplace(char *s, char *what, char *to);

/*
**	print
*/
void					ft_putchar(int c);
void					ft_putchar_fd(int c, int fd);
void					ft_putendl(char const *s);
void					ft_putendl_fd(char const *s, int fd);
void					ft_putnbr(int n);
void					ft_putnbr_fd(int n, int fd);
void					ft_putstr(char const *s);
void					ft_putstr_fd(char const *s, int fd);
void					ft_putwchar(int c);
void					ft_putwchar_fd(int out, int c);

#endif