/**
 *
 * Copyright (c) 1994-1995 Gensym Corporation.  All Rights Reserved.
 *
 * Module:      tlt.h
 *
 * Copyright (c) 1999 The Thinlisp Group
 * All Rights Reserved.
 *
 * This file is part of ThinLisp.
 *
 * ThinLisp is open source; you can redistribute it and/or modify it
 * under the terms of the ThinLisp License as published by the ThinLisp
 * Group; either version 1 or (at your option) any later version.
 *
 * ThinLisp is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * For additional information see <http://www.thinlisp.org/>
 *
 * Author Jim Allard
 *
 * This file contains declarations and externs for the ThinLisp (TL) runtime
 * library.
 */

#include <ctype.h>
#include <math.h>

#if defined(__STDC__)
#  include <string.h>
#endif

#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>


/**
 * Integer types in TL translated code will always refer to the following types,
 * representing signed and unsigned integers containing the given number of
 * bits.  The one except to this rule is the type unsigned char, which may
 * appear in TL translated code and is assumed to be equivalent to uint8.  Twos
 * complement representation of all integers is assumed.
 */

typedef unsigned char  uint8;
typedef   signed char  sint8;
typedef unsigned short uint16;
typedef   signed short sint16;
typedef unsigned int   uint32;
typedef   signed int   sint32;


/**

 * Structure types in TL provide the base level data structure needed to
 * build the rest of the runtime system.
 *
 *   Hdr - The header of all heap allocated Lisp objects, has type tag,
 *   Obj - an unsigned int 32 holding pointers and immediate values,
 *   Sv - heap allocated simple-vectors,
 *   Str - heap allocated strings,
 *   Sa_uint8 - heap allocated (unsigned-byte 8) simple-arrays,
 *   Sa_uint16 - heap allocated (unsigned-byte 16) simple-arrays,
 *   Sa_double - heap allocated double-float simple-arrays,
 *   Ldouble - heap allocated double-floats,
 *   Mdouble - heap allocated managed-floats,
 *   Sym - symbols,
 *   Func - compiled-functions,
 *   Pkg - packages,
 *   String_strm - string-streams,
 *   File_strm - file-streams, and 
 *   Class_hdr - class and structure instances.
 */

typedef struct {
  unsigned int type:  8;
  unsigned int fill: 24;
} Hdr;

#if defined(alphaosf)
typedef uint32 Obj;
#else
typedef void *Obj;
#endif

typedef struct {
  Obj car;
  Obj cdr;
} Cons;

typedef struct {
  unsigned int type:    8;
  unsigned int length: 24;
  Obj body[1];
} Sv;

typedef struct {
  unsigned int type:         8;
  unsigned int length:      24;
  unsigned int fill_length: 24;
  unsigned char body[9];
} Str;

typedef struct {
  unsigned int type:         8;
  unsigned int length:      24;
  unsigned int fill_length: 24;
  uint8 body[4];
} Sa_uint8;

typedef struct {
  unsigned int type:         8;
  unsigned int length:      24;
  unsigned int fill_length: 24;
  uint16 body[2];
} Sa_uint16;

typedef struct {
  unsigned int type:    8;
  unsigned int length: 24;
  double body[1];
} Sa_double;

typedef struct {
  unsigned int type: 8;
  double body;
} Ldouble;

typedef struct {
  unsigned int type: 8;
  union {
    double value;
    Obj    next_object;
  } body;
} Mdouble;

typedef struct {
  unsigned int type:        8;
  unsigned int local_value: 1;
  unsigned int external:    1;
    signed int balance:     4;
  unsigned int imported:    1;
  unsigned int name_hash:  16;
  Obj symbol_name;
  Obj symbol_value;
  Obj symbol_plist;
  Obj symbol_package;
  Obj symbol_function;
  Obj left_branch;
  Obj right_branch;
} Sym;

typedef struct {
  unsigned int type:               8;
  unsigned int arg_count:          8;
  unsigned int optional_arguments: 8;
  unsigned int sets_values_count:  1;
  Obj default_arguments;
  Obj name;
  Obj (*c_function)(Obj);
} Func;

typedef struct {
  unsigned int type:               8;
  unsigned int arg_count:          8;
  unsigned int optional_arguments: 8;
  unsigned int sets_values_count:  1;
  Obj default_arguments;
  Obj name;
  Obj (*c_function)(void);
} Func_0;

typedef struct {
  unsigned int type:               8;
  unsigned int arg_count:          8;
  unsigned int optional_arguments: 8;
  unsigned int sets_values_count:  1;
  Obj default_arguments;
  Obj name;
  Obj (*c_function)(Obj);
} Func_1;

typedef struct {
  unsigned int type:               8;
  unsigned int arg_count:          8;
  unsigned int optional_arguments: 8;
  unsigned int sets_values_count:  1;
  Obj default_arguments;
  Obj name;
  Obj (*c_function)(Obj, Obj);
} Func_2;

typedef struct {
  unsigned int type:               8;
  unsigned int arg_count:          8;
  unsigned int optional_arguments: 8;
  unsigned int sets_values_count:  1;
  Obj default_arguments;
  Obj name;
  Obj (*c_function)(Obj, Obj, Obj);
} Func_3;

typedef struct {
  unsigned int type:               8;
  unsigned int arg_count:          8;
  unsigned int optional_arguments: 8;
  unsigned int sets_values_count:  1;
  Obj default_arguments;
  Obj name;
  Obj (*c_function)(Obj, Obj, Obj, Obj);
} Func_4;

typedef struct {
  unsigned int type:               8;
  unsigned int arg_count:          8;
  unsigned int optional_arguments: 8;
  unsigned int sets_values_count:  1;
  Obj default_arguments;
  Obj name;
  Obj (*c_function)(Obj, Obj, Obj, Obj, Obj);
} Func_5;

typedef struct {
  unsigned int type:               8;
  unsigned int arg_count:          8;
  unsigned int optional_arguments: 8;
  unsigned int sets_values_count:  1;
  Obj default_arguments;
  Obj name;
  Obj (*c_function)(Obj, Obj, Obj, Obj, Obj, Obj);
} Func_6;

typedef struct {
  unsigned int type:               8;
  unsigned int arg_count:          8;
  unsigned int optional_arguments: 8;
  unsigned int sets_values_count:  1;
  Obj default_arguments;
  Obj name;
  Obj (*c_function)(Obj, Obj, Obj, Obj, Obj, Obj, Obj);
} Func_7;

typedef struct {
  unsigned int type:               8;
  unsigned int arg_count:          8;
  unsigned int optional_arguments: 8;
  unsigned int sets_values_count:  1;
  Obj default_arguments;
  Obj name;
  Obj (*c_function)(Obj, Obj, Obj, Obj, Obj, Obj, Obj, Obj);
} Func_8;

typedef struct {
  unsigned int type:               8;
  unsigned int arg_count:          8;
  unsigned int optional_arguments: 8;
  unsigned int sets_values_count:  1;
  Obj default_arguments;
  Obj name;
  Obj (*c_function)(Obj, Obj, Obj, Obj, Obj, Obj, Obj, Obj, Obj);
} Func_9;

typedef struct {
  unsigned int type:               8;
  unsigned int arg_count:          8;
  unsigned int optional_arguments: 8;
  unsigned int sets_values_count:  1;
  Obj default_arguments;
  Obj name;
  Obj (*c_function)(Obj, Obj, Obj, Obj, Obj, Obj, Obj, Obj, Obj, Obj);
} Func_10;

typedef struct {
  unsigned int type:               8;
  unsigned int arg_count:          8;
  unsigned int optional_arguments: 8;
  unsigned int sets_values_count:  1;
  Obj default_arguments;
  Obj name;
  Obj (*c_function)(Obj, Obj, Obj, Obj, Obj, Obj, Obj, Obj, Obj, Obj, Obj);
} Func_11;

typedef struct {
  unsigned int type:               8;
  unsigned int arg_count:          8;
  unsigned int optional_arguments: 8;
  unsigned int sets_values_count:  1;
  Obj default_arguments;
  Obj name;
  Obj (*c_function)(Obj, Obj, Obj, Obj, Obj, Obj, Obj, Obj, Obj, Obj, Obj, Obj);
} Func_12;

typedef struct {
  unsigned int type:               8;
  unsigned int arg_count:          8;
  unsigned int optional_arguments: 8;
  unsigned int sets_values_count:  1;
  Obj default_arguments;
  Obj name;
  Obj (*c_function)(Obj, Obj, Obj, Obj, Obj, Obj, Obj, Obj, Obj, Obj, Obj, Obj,
		    Obj);
} Func_13;

typedef struct {
  unsigned int type:               8;
  unsigned int arg_count:          8;
  unsigned int optional_arguments: 8;
  unsigned int sets_values_count:  1;
  Obj default_arguments;
  Obj name;
  Obj (*c_function)(Obj, Obj, Obj, Obj, Obj, Obj, Obj, Obj, Obj, Obj, Obj, Obj,
		    Obj, Obj);
} Func_14;

typedef struct {
  unsigned int type:               8;
  unsigned int arg_count:          8;
  unsigned int optional_arguments: 8;
  unsigned int sets_values_count:  1;
  Obj default_arguments;
  Obj name;
  Obj (*c_function)(Obj, Obj, Obj, Obj, Obj, Obj, Obj, Obj, Obj, Obj, Obj, Obj,
		    Obj, Obj, Obj);
} Func_15;

typedef struct {
  unsigned int type:               8;
  unsigned int arg_count:          8;
  unsigned int optional_arguments: 8;
  unsigned int sets_values_count:  1;
  Obj default_arguments;
  Obj name;
  Obj (*c_function)(Obj, Obj, Obj, Obj, Obj, Obj, Obj, Obj, Obj, Obj, Obj, Obj,
		    Obj, Obj, Obj, Obj);
} Func_16;

typedef struct {
  unsigned int type:               8;
  unsigned int arg_count:          8;
  unsigned int optional_arguments: 8;
  unsigned int sets_values_count:  1;
  Obj default_arguments;
  Obj name;
  Obj (*c_function)(Obj, Obj, Obj, Obj, Obj, Obj, Obj, Obj, Obj, Obj, Obj, Obj,
		    Obj, Obj, Obj, Obj, Obj);
} Func_17;

typedef struct {
  unsigned int type:               8;
  unsigned int arg_count:          8;
  unsigned int optional_arguments: 8;
  unsigned int sets_values_count:  1;
  Obj default_arguments;
  Obj name;
  Obj (*c_function)(Obj, Obj, Obj, Obj, Obj, Obj, Obj, Obj, Obj, Obj, Obj, Obj,
		    Obj, Obj, Obj, Obj, Obj, Obj);
} Func_18;

typedef struct {
  unsigned int type:               8;
  unsigned int arg_count:          8;
  unsigned int optional_arguments: 8;
  unsigned int sets_values_count:  1;
  Obj default_arguments;
  Obj name;
  Obj (*c_function)(Obj, Obj, Obj, Obj, Obj, Obj, Obj, Obj, Obj, Obj, Obj, Obj,
		    Obj, Obj, Obj, Obj, Obj, Obj, Obj);
} Func_19;

typedef struct {
  unsigned int type:               8;
  unsigned int arg_count:          8;
  unsigned int optional_arguments: 8;
  unsigned int sets_values_count:  1;
  Obj default_arguments;
  Obj name;
  Obj (*c_function)(Obj, Obj, Obj, Obj, Obj, Obj, Obj, Obj, Obj, Obj, Obj, Obj,
		    Obj, Obj, Obj, Obj, Obj, Obj, Obj, Obj);
} Func_20;

typedef struct {
  unsigned int type:  8;
  Obj name;
  Obj root_symbol;
  Obj used_packages;
} Pkg;

typedef struct {
  unsigned int type:  8;
  Obj strings;
  unsigned char *input_string;
  sint32 input_index;
  sint32 input_index_bounds;
} String_strm;

typedef struct {
  unsigned int type:  8;
  FILE       *input;
  FILE       *output;
  char       *filename;
  char       *mode;
} File_strm;

typedef struct {
  unsigned int type:  8;
  unsigned int extended_type: 24;
} Class_hdr;


/**
 * Externs for the hand-written C utilities in tl.c
 */

extern sint32 Values_count;

extern Obj Values_buffer[20];

extern Hdr Unbound;

#define THROW_STACK_MAX 2048

extern Obj Throw_stack[THROW_STACK_MAX];

extern sint32 Throw_stack_top;

extern Obj Current_throw;

extern void store_values_on_stack(Obj first_value);

extern Obj retrieve_values_from_stack(void);

extern void throw_towards_catch_tag(Obj throw_tag, Obj first_value);

extern void malloc_block_into_region(sint32 region, sint32 byte_count,
				     sint32 silent);

extern sint32 region_number_bytes_size(sint32 region);

extern sint32 region_number_bytes_used(sint32 region);

extern sint32 region_number_bytes_available(sint32 region);

extern Obj alloc_cons(Obj new_car, Obj new_cdr, sint32 region);

extern Obj hook_up_cdrs(Obj *cons_array, sint32 count, Obj final_cdr);

extern Obj alloc_list(sint32 length, sint32 init_cars_p, Obj init_elt, sint32 region);

extern Obj alloc_simple_vector(sint32 length, sint32 region, sint32 type_tag);

extern Obj alloc_string(sint32 dimension, sint32 region, sint32 type_tag);

extern Obj alloc_uint8_array(sint32 length, sint32 region, sint32 type_tag);

extern Obj alloc_uint16_array(sint32 length, sint32 region, sint32 type_tag);

extern Obj alloc_double_array(sint32 length, sint32 region, sint32 type_tag);

extern Obj alloc_ldouble(double new_value, sint32 region, sint32 type_tag);

extern Obj alloc_mdouble(double new_value, sint32 region, sint32 type_tag);

extern Obj alloc_symbol(sint32 region, sint32 type_tag);

extern Sym T;

extern Obj alloc_package(Obj name, Obj used, sint32 region, sint32 type_tag);

extern Obj alloc_string_strm(sint32 region, sint32 type_tag);

extern Obj alloc_file_strm(FILE *input, FILE* output, char *filename,
			   char* mode, sint32 region, sint32 type_tag);

extern Obj alloc_struct(sint32 bytes, sint32 align, sint32 region,
			sint32 type_tag);

extern void notify(char *message);

extern void warn(char *message);

extern void error(char *message);

extern void type_cast_error(char *source_type, char *target_type);

extern void fatal_error(char *message);

extern void write_fixnum_into_str(sint32 value, sint32 width, Str *output);

extern void write_double_into_str(double value, sint32 width, Str *output);

extern void init_cronometer(void);

extern sint32 cronometer(void);

extern sint32 cpu_run_time(void);

extern sint32 ticks_per_second(void);

extern void sleep_ticks(sint32 seconds);

/**
 * In some cases it seems that externs have been explicitly left out on some
 * platforms.  The following section implements declarations for those functions
 * to suppress warnings from C compilers.
 */

#if defined(sun4)
extern int printf(const char *format, ...);
extern int fprintf(FILE *stream, const char *format, ...);
extern int fputs(const char*s, FILE *stream);
extern void *memset(void *ptr, int val, size_t len);
extern int _flsbuf(unsigned char c, FILE *f);
extern int _filbuf(FILE *f);
extern int fflush(FILE *stream);
extern int fclose(FILE *stream);
extern int unlink(char *filename);
#endif



/**
 * External and imported functions of DLL libraries on Windows platforms
 * need an extra declaration.  The macros DLLIMPORT and DLLEXPORT abstract
 * those declarations for use within our machine independent C translated
 * files.
 */

#if defined(WIN32)
#define DLLIMPORT __declspec( dllimport )
#define DLLEXPORT __declspec( dllexport )
#else
#define DLLIMPORT
#define DLLEXPORT
#endif


/**
 * This section contains a few defines used by the translator.  The number of
 * defines has purposefully been kept to a minimum in order to keep it clear to
 * the reader of translated C code what the cost is of various operations.
 * However, for a few operations, such as CAR and CDR, there are single machine
 * instruction macros that greatly improve the clarity of the translated code.
 */

#define CAR(cons_as_obj) (((Cons *)((uint32)(cons_as_obj)-2))->car)
#define CDR(cons_as_obj) (((Cons *)((uint32)(cons_as_obj)-2))->cdr)



/**

 * When defining list or simple-vector constants, initialized C Obj arrays are
 * used for constant sub-elements so that the linker can initialize these data
 * structures rather than having it occur at process launch time.  This works
 * fine, except on the Alpha OSF platform, where we've used 32-bit fields to
 * store addresses, which usually are 64 bit.  We've made that work by using an
 * Alpha OSF linker option which will guarantee that all significant bits of
 * addresses can be stored in the lower 32 bits of an address.  This hack cuts
 * our runtime memory usage in half.  However, the Alpha OSF compiler refuses to
 * do "narrowing" type casts in constant expressions.  Therefore, on this
 * platform we had to use runtime initialization of constant pointers rather
 * than the linker time version.  The NO_ADDRESS_CONSTANTS define was used to
 * select which approach was used.
 *
 * Since we currently have no users on the Alpha OSF, I'm commenting all code
 * that dealt with this function so that the emitted C code is simplified.
 * Search for NO_ADDRESS_CONSTANTS in tlt/lisp to find relevant code if you wish
 * to re-enable this feature.  -jallard, 10/31/99
 */

/* #if defined(__osf__)
 * #  define NO_ADDRESS_CONSTANTS 1
 * #endif
 */

/**
 * The macro BOXFIX takes sint32 values and convert them to fixnum format (left
 * shift by 2 and add the immediate type tag 1).  The macro UNBOXFIX does the
 * inverse transform.  The macro BOXCHAR and UNBOXCHAR do the same for unsigned
 * char and Lisp characters (immediate type tag of 3).  */

#define BOXFIX(any_int) (Obj)(((uint32)(any_int)<<2)+1)
#define UNBOXFIX(fixnum_int) ((sint32)(fixnum_int)>>2)

#define BOXCHAR(char_int) (Obj)((((uint32)(char_int)&0xff)<<2)+3)
#define UNBOXCHAR(character) (unsigned char)((uint32)(character)>>2)

/**
 * The macro StrHDR() takes a pointer to an unsigned char, and returns a Str* to
 * the Str structure that contains it.  ObjStrHDR() does the same, but returns
 * it as type Obj.  SvHDR takes a pointer to the Obj array in a simple-vector,
 * and returns an Sv* to its containing header.  ObjSvHDR() does the same, but
 * returning it as an Obj.
 */

#define StrHDR(string) ((Str *)((uint32)(string) - (uint32)(&(((Str *)NULL)->body[0]))))
#define ObjStrHDR(string) ((Obj)((uint32)(string) - (uint32)(&(((Str *)NULL)->body[0]))))

#define SvHDR(obj_ptr) ((Sv *)((uint32)(obj_ptr) - sizeof(Hdr)))
#define ObjSvHDR(obj_ptr) ((Obj)((uint32)(obj_ptr) - sizeof(Hdr)))




/**
 * The macro TYPE_TAG takes a Lisp Obj and an sint32 temporary variable, and
 * returns its int type tag.  
 */

#define CLASS_HDR_TAG 17

#define IMMED_TAG(object) ((uint32)(object) & 3)

#define STD_TAG(object) (((Hdr *)(object))->type)

#define EXTENDED_TAG(object) (((Class_hdr *)(object))->extended_type)

#define TYPE_TAG(object,temp_var) ((object) == NULL ? 0 : \
  (temp_var = IMMED_TAG(object)) != 0 ?  temp_var : \
  (temp_var = STD_TAG(object)) != CLASS_HDR_TAG ? temp_var : \
  EXTENDED_TAG(object))
