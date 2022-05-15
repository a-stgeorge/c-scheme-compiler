(define zero?
  (lambda (x)
    (= x 0)))

(define list
  (lambda args
    args))

(define append
  (lambda (l1 l2)
    (if (null? l1)
        l2
        (cons (car l1) (append (cdr l1) l2)))))

(define caar
  (lambda (lst)
    (car (car lst))))

(define cadr
  (lambda (lst)
    (car (cdr lst))))

(define cdar
  (lambda (lst)
    (cdr (car lst))))

(define cddr
  (lambda (lst)
    (cdr (cdr lst))))

(define caaar
  (lambda (lst)
    (car (car (car lst)))))

(define caadr
  (lambda (lst)
    (car (car (cdr lst)))))

(define cadar
  (lambda (lst)
    (car (cdr (car lst)))))

(define caddr
  (lambda (lst)
    (car (cdr (cdr lst)))))

(define cdaar
  (lambda (lst)
    (cdr (car (car lst)))))

(define cdadr
  (lambda (lst)
    (cdr (car (cdr lst)))))

(define cddar
  (lambda (lst)
    (cdr (cdr (car lst)))))

(define cdddr
  (lambda (lst)
    (cdr (cdr (cdr lst)))))

(define caaaar
  (lambda (lst)
    (car (car (car (car lst))))))

(define caaadr
  (lambda (lst)
    (car (car (car (cdr lst))))))

(define caadar
  (lambda (lst)
    (car (car (cdr (car lst))))))

(define caaddr
  (lambda (lst)
    (car (car (cdr (cdr lst))))))

(define cadaar
  (lambda (lst)
    (car (cdr (car (car lst))))))

(define cadadr
  (lambda (lst)
    (car (cdr (car (cdr lst))))))

(define caddar
  (lambda (lst)
    (car (cdr (cdr (car lst))))))

(define cadddr
  (lambda (lst)
    (car (cdr (cdr (cdr lst))))))

(define cdaaar
  (lambda (lst)
    (cdr (car (car (car lst))))))

(define cdaadr
  (lambda (lst)
    (cdr (car (car (cdr lst))))))

(define cdadar
  (lambda (lst)
    (cdr (car (cdr (car lst))))))

(define cdaddr
  (lambda (lst)
    (cdr (car (cdr (cdr lst))))))

(define cddaar
  (lambda (lst)
    (cdr (cdr (car (car lst))))))

(define cddadr
  (lambda (lst)
    (cdr (cdr (car (cdr lst))))))

(define cdddar
  (lambda (lst)
    (cdr (cdr (cdr (car lst))))))

(define cddddr
  (lambda (lst)
    (cdr (cdr (cdr (cdr lst))))))

(define race
  (lambda (hare tortoise)
    (if (pair? hare)
        (let ((hare (cdr hare)))
          (if (pair? hare)
              (and (not (eq? hare tortoise))
                   (race (cdr hare) (cdr tortoise)))
              (null? hare)))
        (null? hare))))

(define list?
  (lambda (x)
    (race x x)))

(define length
  (lambda (lst)
    (if (null? lst)
        0
        (+ 1 (length (cdr lst))))))

(define list-ref
  (lambda (ls n)
    (if (= n 0)
        (car ls)
        (list-ref (cdr ls) (- n 1)))))

(define list-tail
  (lambda (ls n)
    (if (= n 0)
        ls
        (list-tail (cdr ls) (- n 1)))))

(define member
  (lambda (x ls)
    (cond
      ((null? ls) #f)
      ((equal? (car ls) x) ls)
      (else (memq x (cdr ls))))))

(define assq
  (lambda (x ls)
    (cond
      ((null? ls) #f)
      ((eq? (caar ls) x) (car ls))
      (else (assq x (cdr ls))))))

(define reverse
  (lambda (lst)
    (if (null? lst)
        '()
        (append (reverse (cdr lst))
                (list (car lst))))))

(define map
  (lambda (p ls)
    (if (null? ls)
        '()
        (cons (p (car ls))
              (map p (cdr ls))))))

(define filter
  (lambda (predicate lst)
    (cond ((null? lst) '())
          ((predicate (car lst)) (cons (car lst) (filter pred (cdr lst))))
          (else (filter predicate (cdr lst))))))

(define foldl
  (lambda (f init lst)
    (if (null? lst)
        init
        (foldl f
               (f (car lst) init)
               (cdr lst)))))

(define foldr
  (lambda (f init lst)
    (if (null? lst)
        init
        (f (car lst)
           (foldr f
                  init
                  (cdr lst))))))
