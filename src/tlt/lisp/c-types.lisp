(in-package "GLI")

;;;; Module C-TYPES

;;; Copyright (c) 1999 The ThinLisp Group
;;; Copyright (c) 1995 Gensym Corporation.
;;; All rights reserved.

;;; This file is part of ThinLisp.

;;; ThinLisp is open source; you can redistribute it and/or modify it
;;; under the terms of the ThinLisp License as published by the ThinLisp
;;; Group; either version 1 or (at your option) any later version.

;;; ThinLisp is distributed in the hope that it will be useful, but
;;; WITHOUT ANY WARRANTY; without even the implied warranty of
;;; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

;;; For additional information see <http://www.thinlisp.org/>

;;; Author: Jim Allard






;;;; C Types




;;; This module implements operations on C types.

;;; `C types' are represented by symbols and lists of symbols.  The following
;;; symbols are available:

;;;   void, which is nothing,
;;;   Obj which is an sint32 whose value points to a Hdr (aka header),
;;;   Hdr is a structure holding a type tag,
;;;   sint32 which is a signed 32-bit integer,
;;;   uint8 which is an unsigned 8-bit integer (equivalent to unsigned-char),
;;;   uint16 which is an unsigned 16-bit integer (aka unsigned short),
;;;   unsigned-char which is C unsigned char (equivalent to uint8),
;;;   boolean which is a C int used as an arg to logical predicates,
;;;   Mdouble which is a structure holding a managed-float (tag 0x4),
;;;   Ldouble which is a structure holding a double-float (tag 0x5),
;;;   Sv which is a simple-vector structure (tag 0x6),
;;;   Str which is a string structure (tag 0x7),
;;;   Sa_unit8 which is a structure holding uint8 arrays (tag 0x8),
;;;   Sa_unit16 which is a structure holding uint16 arrays (tag 0x9),
;;;   Sa_double which is a structure holding double arrays (tag 0xA),
;;;   Sym which is a symbol structure (tag 0xB),
;;;   Func which is compiled-function structure (tag 0xC),
;;;   Pkg which is a package structure (tag 0xD), and
;;;   jmp_buf which is a jump buffer for setjmp and longjmp.

;;; C types can also be lists.  In these cases the car of the list must be one
;;; of the following symbols, with the format of the remainder of the list
;;; determined by the particular symbol.

;;;   (pointer <c-type>) is a pointer to the enclosed C type, and
;;;   (array <c-type> [<array-length>]) is an array of the C type.
;;;   (const-array <c-type> <array-length>) is described below.
;;;   (c-type "<c-type-string>" | (pointer "<c-type-string>")) described below.

;;; Const-array is a type used for emitting new type definitions to initialize
;;; Lisp arrays of constant sizes.  For example, the type Sv is a C structure
;;; representing a Lisp simple-vector.  The actual Obj array in the Sv structure
;;; is only 1 element long, but since C explicitly performs no array bounds
;;; checking we can use this structure to access arbitrarily sized arrays in
;;; data structures we create by allocating more memory beyond the bounds of the
;;; usual Sv struct size.  If however we want to have a C variable holding an
;;; initialized instance of this structure, then we need to have a C type with
;;; the correct number of elements in the Obj array embedded within the struct.
;;; That is where the const-array C type comes in.  When a new type is needed
;;; for an explicitly sized embedded array within a structure, the const-array C
;;; type will be used.  The C type that is the second of the list will be the
;;; structure type whose embedded array is being specialized.  The size that is
;;; the third of the list is the length of the embedded array.  If this C type
;;; is used in a c-typedef-decl, the generated type name will be the normal
;;; structure name with "_<length>" appended to it, e.g. Sv_5.

;;; C-type is a type used for GL end-users extending the C types that GL can
;;; handle.  They contain either the string naming C type, or the list (pointer
;;; "<c-name>").  These C types are exactly the same form as the Lisp type that
;;; represents them.

;;; The macro `c-type-string' takes a C type and returns a C type string
;;; suitable for use in a type casting operation.  New type translations can be
;;; made using def-c-type.

(defmacro c-type-string (c-type)
  (let* ((type (if (symbolp c-type) c-type (gensym)))
	 (prefix (if (eq type c-type) '(progn) `(let ((,type ,c-type))))))
    `(,@prefix
       (if (symbolp ,type)
	   (get ,type 'c-type-string)
	   (get-compound-c-type-string ,type)))))

(defun get-compound-c-type-string (c-type)
  (let ((car (cons-car c-type)) 
	(second (cons-second c-type)))
    (cond ((eq car 'c-type)
	   (cond ((stringp second)
		  second)
		 ((and (consp second)
		       (eq (cons-car second) 'pointer)
		       (stringp (cons-second second)))
		  (format nil "~a *" (cons-second second)))
		 (t
		  (error "Can't make type string for ~s" c-type))))
	  ((or (eq car 'pointer) (eq car 'array))
	   (if (symbolp second)
	       (get second 'c-pointer-type-string)
	       (format nil "(~a) *" (c-type-string second))))
	  ((eq car 'const-array)
	   (format nil "~a_~a" (c-type-string second) (third c-type)))
	  ((eq car 'function)
	   (let ((return-type second)
		 (arg-types (cons-third c-type)))
	     (with-output-to-string (out)
	       (format out "~a (*)(" (c-type-string return-type))
	       (if (null arg-types)
		   (format out "void")
		   (loop for first? = t then nil
			 for arg-type in arg-types
			 do
		     (unless first?
		       (format out ", "))
		     (format out "~a" (c-type-string arg-type))))
	       (format out ")"))))
	  (t
	   (error "Can't make type string for ~s" c-type)))))




;;; The macro `c-types-equal-p' takes two C types and returns whether or not
;;; they are equivalent.

(defmacro c-types-equal-p (type1 type2)
  `(equal ,type1 ,type2))




;;; The macro `satisfies-c-required-type-p' takes a result C type and a required
;;; C type, and then this function returns whether or not the result type is
;;; appropriate as a value to an operation needing the required-type.

(defmacro satisfies-c-required-type-p (result-type required-type)
  (let ((result (make-symbol "RESULT"))
	(required (if (constantp required-type)
		      required-type
		      (make-symbol "REQUIRED"))))
    `(let ((,result ,result-type)
	   ,@(if (not (eq required required-type))
		 `((,required ,required-type))))
       (or (eq ,required 'void)
	   (equal ,result ,required)
	   ,@(if (or (not (constantp required-type))
		     (consp (eval required-type)))
		 `((and (consp ,result)
			,@(if (not (constantp required-type))
			      `((consp ,required)))
			(memqp (cons-car ,result) '(pointer array))
			(memqp (cons-car ,required) '(pointer array))
			(equal (cons-second ,result)
			       (cons-second ,required)))))))))




;;; The function `c-type-tag' takes a C type and returns the type tag integer
;;; for that type, if any.  The function `c-type-implementing-lisp-type' takes a
;;; Lisp type and returns a C type symbol, where a pointer to that C type
;;; implements the given Lisp type.  The function `lisp-type-tag' takes a Lisp
;;; type and returns an integer which is the type tag for that Lisp type.

(defun c-type-tag (c-type)
  (or (if (symbolp c-type)
	  (get c-type 'c-type-tag)
	  nil)
      (translation-error "C type ~s has no type tag." c-type)))

(defvar c-types-implementing-lisp-type-alist nil)

(defun c-type-implementing-lisp-type (lisp-type)
  (cond ((symbolp lisp-type)
	 (get lisp-type 'c-type-implementing-lisp-type))
	((and (consp lisp-type)
	      (eq (cons-car lisp-type) 'c-type))
	 lisp-type)
	(t
	 (loop for entry in c-types-implementing-lisp-type-alist
	       do
	   (when (gl-subtypep lisp-type (cons-car entry))
	     (return (cons-cdr entry)))))))

(defun lisp-type-tag (lisp-type)
  (if (eq lisp-type 'null)
      0
      (let ((c-type (c-type-implementing-lisp-type lisp-type)))
	(if (symbolp c-type)
	    (get c-type 'c-type-tag)
	    nil))))




;;; The function `type-tags-for-lisp-type' takes a Lisp type and returns a list
;;; of the type tags that could appear on objects of that type.

(defun type-tags-for-lisp-type (lisp-type)
  (let ((tag-list nil))
    (setq lisp-type (expand-type lisp-type))
    (cond ((and (consp lisp-type) (eq (cons-car lisp-type) 'or))
	   (loop for type in (cons-cdr lisp-type) do
	     (loop for subtype-tag in (type-tags-for-lisp-type type) do
	       (pushnew subtype-tag tag-list))))
	  (t
	   (loop for (type . tag)
		     in '((null . 0) (fixnum . 1) (cons . 2) (character . 3))
		 do
	     (when (gl-subtypep type lisp-type)
	       (push tag tag-list)))
	   (loop for (type . c-type) in c-types-implementing-lisp-type-alist do
	     (when (gl-subtypep type lisp-type)
	       (push (c-type-tag c-type) tag-list)))))
    (sort (the list tag-list) #'<)))
    






;;; The function `c-type-for-const-array' takes a C type that holds
;;; implementations of Lisp array types, and the length of a constant array.
;;; This function will return the appropriate C const-array type for that
;;; constant.  Generally this involves rounding up the length to some
;;; appropriate value.  ANSI C requires that the array types be at least one
;;; element long.

(defun c-type-for-const-array (c-type length)
  (when (zerop length)
    (setq length 1))
  (list
    'const-array
    c-type
    (cond
      ((satisfies-c-required-type-p c-type 'str)
       ;; Each element is 1 byte long, so it makes sense to round up to word
       ;; sizes.  We need one extra element for the NULL byte at the end of the
       ;; C string.
       (+ (round-up length 4) 1))
      ((satisfies-c-required-type-p c-type 'sa-uint8)
       (round-up length 4))
      ((satisfies-c-required-type-p c-type 'sa-uint16)
       (round-up length 2))
      ((satisfies-c-required-type-p c-type 'sa-double)
       length)
      ((satisfies-c-required-type-p c-type 'sv)
       length))))




;;; The function `c-func-type-holding-function-type' takes a C function-type and
;;; returns a variant of the C Func type which happens to hold pointers to
;;; functions of the given description.  This is similar to what is done for C
;;; constant arrays of Lisp types, but all of the currently defined C Func types
;;; are defined explicitly in gl/c/glt.h.  This function signals an error if it
;;; cannot return an appropriate type.

(defun c-func-type-holding-function-type (c-function-type)
  (let* ((c-return-type (cons-second c-function-type))
	 (c-arg-types (cons-third c-function-type))
	 (arg-count (length c-arg-types)))
    (declare (fixnum arg-count))
    (unless (and (eq c-return-type 'obj)
		 (loop for type in c-arg-types
		       always (eq type 'obj)))
      (translation-error "Cannot find appropriate function type for spec ~s"
			 c-function-type))
    (unless (<= arg-count gl:lambda-parameters-limit)
      (translation-error
	"Funcalls are limited to ~a arguments, a call had ~a."
	gl:lambda-parameters-limit arg-count))
    ;; The case statement optimizes the most common cases, but otherwise is
    ;; unneccesary.
    (list 'pointer
	  (case arg-count
	    (0 'func-0)
	    (1 'func-1)
	    (2 'func-2)
	    (3 'func-3)
	    (4 'func-4)
	    (5 'func-5)
	    (6 'func-6)
	    (7 'func-7)
	    (8 'func-8)
	    (9 'func-9)
	    (10 'func-10)
	    (t
	     (intern (format nil "FUNC-~a" arg-count) *gli-package*))))))




;;; The macro `def-c-type' defines Lisp symbols that correspond to given C
;;; types.  It takes a Lisp symbol naming a C type, a string of how that type
;;; should be printed in C files, and a string of how a pointer to that type
;;; should be printed in C files.  The last two arguments are non-NIL if a
;;; pointer to this type represents a Lisp type.  The first is the Lisp type
;;; represented by this C type and the last argument is a type-tag number if a
;;; pointer to this C type represents a Lisp type.

(defmacro def-c-type
    (type-name c-type-string c-pointer-type-string lisp-type? type-tag?)
  `(progn
     (setf (get ',type-name 'c-type-string) ,c-type-string)
     (setf (get ',type-name 'c-pointer-type-string) ,c-pointer-type-string)
     ,@(when lisp-type?
	 `(,@(when (symbolp lisp-type?)
	       `((setf (get ',lisp-type? 'c-type-implementing-lisp-type)
		       ',type-name)))
	     (push (cons ',lisp-type? ',type-name)
		   c-types-implementing-lisp-type-alist)
	     (setf (get ',type-name 'c-type-tag) ,type-tag?)))
     ',type-name))
