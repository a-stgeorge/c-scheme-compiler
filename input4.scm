;; These should fail but not crash

;(define x)
;(define 3 5)
;(lambda (x x) x)
(define x (lambda () (quote (1 2 3))))
(x)
