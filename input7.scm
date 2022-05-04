; test of cons

(cons 1 3)

(define x (cons 1 2))
(define y (cons 3 x))
(define z (cons 4 y))
(define a (cons x z))
x
y
z
a

;(cons 1 3 5)
