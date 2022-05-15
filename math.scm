(define >=
  (lambda (a b)
    (if ((<= a b) (= a b))
          #f)))

(define <
  (lambda (a b)
    (not (>= a b))))

(define >
  (lambda (a b)
    (not (<= a b))))

(define zero?
  (lambda (x)
    (= x 0)))

(define positive?
  (lambda (x)
    (> x 0)))

(define negative?
  (lambda (x)
    (< x 0)))

(define even?
  (lambda (x)
    (= (modulo x 2) 0)))

(define odd?
  (lambda (x)
    (not (even? x))))

(define max
  (lambda (a b)
    (if (> a b)
        a
        b)))

(define min
  (lambda (a b)
    (if (< a b)
        a
        b)))

(define abs
  (lambda (x)
    (if (negative? x)
        (* -1 x)
        (x))))

(define gcd
  (lambda (a b)
    (if (= b 0)
        a
        (gcd b (modulo a b)))))

(define lcm
  (lambda (a b)
    (/ (* a b) (gcd a b))))

(define floor
  (lambda (x)
    (cond ((and (<= 0 x) (< x 1)) 0)
          ((>= x 1) (+ 1 (floor-1 (- x 1))))
          (else (- (* -1 (floor-1 (* -1 x))) 1)))))

(define ceiling
  (lambda (x)
    (if (= x 0)
        0
        (+ 1 (floor-1 x)))))

(define truncate
  (lambda (x)
    (if (>= x 0)
        (floor x)
        (ceiling x))))

(define round
  (lambda (x)
    (if (>= x 0)
        (if (> (- x (floor x)) 0.5)
            (ceiling x)
            (floor x))
        (* -1 (round (* -1 x))))))
