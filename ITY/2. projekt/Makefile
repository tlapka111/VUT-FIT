FILENAME = proj2

all :
	make $(FILENAME).pdf
	make clean

$(FILENAME).pdf: $(FILENAME).tex
	for number in 1 2 ; do \
		pdflatex $(FILENAME).tex; \
	done; \

clean:
	rm -f $(FILENAME).aux $(FILENAME).dvi $(FILENAME).log $(FILENAME).out


	




  

  

