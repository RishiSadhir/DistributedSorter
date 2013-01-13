all: sorter splitmerge quick shell bubble

sorter:
	gcc root.c -o sorter

splitmerge:
	gcc splitmerge.c -o splitmerge

quick:
	gcc quicksort.c -o quick

shell:
	gcc shellsort.c -o shell

bubble:
	gcc bubblesort.c -o bubble
