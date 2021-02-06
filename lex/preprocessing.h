#ifndef GCC_PREPROCESSING_H
#define GCC_PREPROCESSING_H GCC_PREPROCESSING_H

void do_preproc_stuff(struct Source_File *src,struct Program *prog);
void do_include_stuff(struct Source_File *src,struct Program *prog);
void do_define_stuff(struct Source_File *src,struct Program *prog);
void handle_splicing(struct token *word);
struct define_directive* get_define_directive(struct token* macro_name);


#endif
