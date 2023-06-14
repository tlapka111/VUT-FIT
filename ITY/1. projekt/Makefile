FILENAME = proj1

all :
	make $(FILENAME).pdf
	make $(FILENAME).ps
	make $(FILENAME).dvi
	make clean

$(FILENAME).pdf: $(FILENAME).ps
	ps2pdf $(FILENAME).ps

$(FILENAME).ps: $(FILENAME).dvi
	dvips -t a4 $(FILENAME).dvi

$(FILENAME).dvi: $(FILENAME).tex
	for number in 1 2 ; do \
		latex --verbose $(FILENAME).tex; \
	done; \

clean:
	rm -f $(FILENAME).aux $(FILENAME).dvi $(FILENAME).log $(FILENAME).out $(FILENAME).ps


	




  

  

