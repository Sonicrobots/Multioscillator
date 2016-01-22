#! /usr/bin/python

import numpy as np
import matplotlib	
import math
import matplotlib.pyplot as plt

def steppedFreq(freq, prescaler=265, clock=16000000, floatCalc=False):
	
	freq = int(freq)
	prescaler = int(prescaler)
	if floatCalc:
		compval = round(float(clock)/(2*prescaler*freq))
	else:
		compval = int(int(int((clock)/(2*prescaler)) + int(freq/2)) / freq)
	compval = min(compval,2**16)
	return clock/prescaler/(2*compval)



if __name__ == "__main__":
	
	freqs = np.logspace(math.log10(20),math.log10(20000),1000,endpoint=True)
	freqs_stepped = [steppedFreq(f)/f for f in freqs]
	freqs_stepped_float = [steppedFreq(f,floatCalc=True)/f for f in freqs]

	plt.figure()
	plt.xlim([20,20000])
	plt.xscale('log')
	plt.ylim([0.9,1.1])

	plt.plot(freqs,freqs_stepped,label="fixed point")
	plt.plot(freqs,freqs_stepped_float,label="floating point")
	
	plt.xlabel("Frequency in Hz")
	plt.ylabel("Relative Deviation")
	
	plt.legend()
	
	plt.show()

	


