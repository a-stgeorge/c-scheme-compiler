(let ((foo "hello")
      (bar (quote foo))
      (baz #f))
  (if baz foo bar))
