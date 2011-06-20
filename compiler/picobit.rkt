#lang racket

(require "utilities.rkt"
         "ast.rkt"
         "env.rkt"
         "reader.rkt"
         "parser.rkt"
         "front-end.rkt"
         "context.rkt"
         "comp.rkt"
         "encoding.rkt"
         "analysis.rkt"
         "scheduling.rkt"
         "tree-shaker.rkt")

;-----------------------------------------------------------------------------

(define (compile filename)
  (let* ((forms (read-file  filename))
         (node  (parse-file forms (make-global-env)))
         (hex-filename
          (path-replace-suffix filename ".hex")))
    
    (adjust-unmutable-references! node)

    (let* ((ctx  (comp-none node (make-init-context)))
           (code (context-code ctx))
           (bbs  (code->vector code)))

      (resolve-toplevel-labels! bbs)

      (let ([bbs  (tree-shake! bbs)]
            [prog (linearize bbs)])
        ;; r5rs's with-output-to-file (in asm.rkt) can't overwrite. bleh
        (when (file-exists? hex-filename)
          (delete-file hex-filename))
        (assemble prog hex-filename)))))


(void (compile (vector-ref (current-command-line-arguments) 0)))