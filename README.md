FILTERING

	make

	./filters.out imageFile sizeOfFilter numberOfPasses filterType >newImageName

	For gaussian:
		- Filter type 0
		./filters.out boat_noise2.pgm 3 3 0 >gaussianresult.pgm

		Filter type 1
		./filters.out boat_noise2.pgm 3 3 1 >medianresult.pgm



HISTOGRAMS

	Work in progress..