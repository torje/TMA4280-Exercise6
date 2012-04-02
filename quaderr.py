#!/usr/bin/python
from math import *
def quaderr():
	print "The error is multiplied by the square of the dimension of the grid it was calculated from"
	A = 4.476185e-02, 1.098931e-02, 2.734955e-03, 6.829684e-04, 1.706940e-04, 4.267049e-05, 1.066744e-05, 2.666847e-06
	b=2
	b=8
	for a in A:
		print a*b**2
		b=b*2
	print "We see it is about equal and thereby it is quadratic convergence in our function"

quaderr()
