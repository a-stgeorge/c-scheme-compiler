;; Tests of =, modulo, zero?, equal?, list, and append (and also load)

"Tests of equal?"
(equal? 3 3)
(equal? 3 (+ 2 1))
(equal? 3 3.0)
(equal? 3 (/ 6 2))
(equal? 3 (/ 6 2.0))
(equal? (quote (to be)) (quote (to be)))
(equal? (quote (to be)) (quote (not to be)))
(equal? (quote (to be)) (quote ((or) (not (to)) be)))
;(equal? 3 3 3)  ;; invalid

"Tests of ="
(= 3 3)
(= 3 (+ 2 1))
(= 3 3.0)
(= 3 (/ 6 2))
(= 65343 (/ 282543132 4324)) ;; big numbers !
(= 23 24)
(= 2 2 2 2)
(= 2 2 2 3)
(= 2 3 2 2)
(= 3 2 2 2)
;(= (= 3 4) 4)
;(= (quote (to be)) (quote (to be)))  ;; yes, this will give an errora

"Tests of modulo"
(modulo 51 17)
(modulo 6 4)
(modulo 12345678 1000)
;(modulo 2 2 2)		;;invalid
;(modulo 5 0)		;;invalid

(load "library.scm")

"Tests of zero?"
(zero? 0)
(zero? 0.0)
(zero? 1)
(zero? (- 3 3))
;(zero? "0")	;; invalid
;(zero? 0 0)	;; invalid

"Tests of list"
(list 1 2 3 4)
(list 1 2 3 (quote (4 5)))
(list 1)
(list)
(list (quote ()))
(list 2 (quote ()))
;(list 1 2 3 (4 5)) ;; invalid

"Tests of append"
(define x (quote (1 2 3)))
(define y (list 2 3 (list 3 5)))
(append (quote (1)) (quote (2)))
(append x y)
(append x 3)
(append x)
(append x x x)
(append)
(append "hello" x)


