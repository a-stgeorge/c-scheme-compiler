(and #t #t #t)
(and #t #f #t)

(or #f #f #t)
(or #f)

; The following should return 1 as a double
(let ((a 4)
      (b 0)
      (c 0)
      (d 0))
  (begin (set! b (+ a a))
         (set! d
               (begin  (set! a 12)
                       (set! c (+ a b))
                       (- c 4)))
  (/ (- (+ d a) b) c)))

; The following should return #t
(letrec
    ((all-true (lambda args
                 (if (null? args)
                     #t
                     (and (car args) (all-true-helper (cdr args))))))
     (all-true-helper (lambda (lst)
                        (if (null? lst)
                            #t
                            (and (car lst) (all-true-helper (cdr lst)))))))
    (all-true #t (= 3 3) (= (/ 4 2) 2)))

; The following should return 57
(let* ((x 12)
       (random
        (lambda args
          (cond (#f 0)
                ((null? args) 1)
                ((null? (cdr args)) (/ x 3))
                (else (set! x 8) (+ x x)))))
       (y x)
       (x 8))
  (+ (random) (random 1) (random 1 2) (random 1 2 3) x y))

; The following should return a void type
(cond (#f 0))
