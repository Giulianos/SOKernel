include Makefile.inc

all: sampleCodeModule idleModule helpModule fortuneModule clearModule snakeModule aModule bModule

idleModule:
		cd idleModule; make

helpModule:
		cd helpModule; make

clearModule:
		cd clearModule; make

fortuneModule:
		cd fortuneModule; make

sampleCodeModule:
	cd SampleCodeModule; make

snakeModule:
	cd snakeModule; make

aModule:
	cd aModule; make

bModule:
	cd bModule; make

clean:
	cd SampleCodeModule; make clean
	cd helpModule; make clean
	cd idleModule; make clean
	cd snakeModule; make clean
	cd aModule; make clean
	cd bModule; make clean
	rm -rf *.bin


.PHONY: idleModule sampleCodeModule helpModule all clean fortuneModule clearModule snakeModule aModule bModule
