(define not
  (lambda (bool)
	(if bool #f #t)))

(define f
  (lambda (x y z)
	(let ((cond (not x)))
	  (if cond y z))))


(f #f 2 3)

(f (not #f) 4 5)
