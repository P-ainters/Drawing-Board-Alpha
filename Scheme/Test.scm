# ONLY USE FOR TEST

(define (foo x) (+ x 1))
(define (add x y) (+ x y))
(define (Cons x y) (lambda (i) (if (not (= i 0)) x y)))
(define (Car pair) (pair 1))
(define (Cdr pair) (pair 0))

(define (SUM n acc)
 (define (x a) a)
 (x 5)
 (if (< n 0) acc (SUM (- n 1) (+ acc n))))

(SUM 10 0)

(SUM 100 0)

# (#painter (line (cons 1 1) (cons 5 5)))
# (#painter (circle (cons 500 500) 200))

# (line-painter default)
