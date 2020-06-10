# ThinLisp 

[Original README](src/README)

### Build and run examples
```
;;; Translate ThinLisp library - generates C sources in tlt
(load "boot.lisp")

;;; Define convenience forms for all systems (add your own to this list).
(def-system-convenience-forms lecho)

;;; Translate your system (modify to compile or translate your system).
(translate-lecho)

```
