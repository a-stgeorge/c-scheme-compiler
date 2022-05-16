;; Library functions that have been defined in scheme

(define zero?
  (lambda (x)
    (= x 0)))

(define list
  (lambda args
    args))

(define append-help		;; Helper function for append
  (lambda (lst1 lst2)
    (if (null? lst1)
        lst2
        (cons (car lst1)
              (append-help (cdr lst1)
                        lst2)))))

(define append
  (lambda lsts
    (if (null? lsts)
        (quote ())
        (append-help (car lsts)
                  (apply append (cdr lsts))))))
