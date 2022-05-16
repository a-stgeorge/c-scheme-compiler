;; Tests of =, modulo, zero?, equal?, list, and append

"Tests of equal?"
(equal? 3 3)
(equal? 3 (+ 2 1))
(equal? 3 3.0)
(equal? 3 (/ 6 2))
(equal? (quote (to be)) (quote (to be)))
(equal? (quote (to be)) (quote (not to be)))
(equal? (quote (to be)) (quote ((or) (not (to)) be)))
;(equal? 3 3 3)  ;; invalid

"Tests of eq?"
(eq? 3 3)
(eq? 3 (+ 2 1))
(eq? 3 3.0)
(eq? 3 (/ 6 2))
(eq? (quote (to be)) (quote (to be)))
(eq? (quote (to be)) (quote (not to be)))

"Tests of ="
(= 3 3)
(= 3 (+ 2 1))
(= 3 3.0)
(= 3 (/ 6 2))
(= 65343 (/ 282543132 4324)) ;; big numbers !
(= 23 24)
;(= (= 3 4) 4)
;(= (quote (to be)) (quote (to be)))  ;; yes, this will give an error
