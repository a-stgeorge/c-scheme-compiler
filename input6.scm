; Tests of car, cdr, and null?

"car and cdr tests"
(define val (quote ((1) 2 3 4)))
(cdr val)
(car val)
(cdr (cdr (cdr (cdr val))))

(define val2 (quote ((((2) 3) 4) 5)))
(car val2)
(car (car (car (car val2))))
(cdr val2)
(cdr (cons 1 3))
;(car 5) ; invalid list
;(cdr (quote 5)) ; invalid list

"null? tests"
(define x (quote ()))
(null? 4)
(null? (+ 2 3))
(null? x)
(null? (lambda () 2))
(null? "hello")
(null? "gfsa")
(null? (quote (3 2)))
(null? (quote ()))
