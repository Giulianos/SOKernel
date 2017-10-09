include Makefile.inc

all: sampleCodeModule shellModule helpModule fortuneModule clearModule snakeModule blobs

shellModule:
		cd shellModule; make

helpModule:
		cd helpModule; make

blobs:
		cd blobsWarModule ; make

clearModule:
		cd clearModule; make

fortuneModule:
		cd fortuneModule; make

sampleCodeModule:
	cd SampleCodeModule; make

snakeModule:
	cd snakeModule; make

clean:
	cd SampleCodeModule; make clean
	cd helpModule; make clean
	cd shellModule; make clean
	cd snakeModule; make clean
	cd blobs; make clean
	rm -rf *.bin


.PHONY: shellModule sampleCodeModule helpModule all clean fortuneModule clearModule snakeModule blobs
