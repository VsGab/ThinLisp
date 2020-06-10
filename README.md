# ThinLisp 

[Original README](src/README)

### Build and run examples
The following steps were tested with CLISP 2.49
The main limitation of the generator currently seems to be the the usage of uint32 for pointers, which makes generated code usable for 32-bit targets only.


From CLISP REPL:

```
;;; Translate ThinLisp library - generates C sources in tlt
(load "boot.lisp")

;;; Define convenience forms for all systems (add your own to this list).
(def-system-convenience-forms lecho)

;;; Translate your system (modify to compile or translate your system).
(translate-lecho)
```

Build 32-bit libtl.a and lecho binary
```
cd tl/bin
make -f makefile-linux
cd ../../lecho/bin/
make -f makefile-linux
```
