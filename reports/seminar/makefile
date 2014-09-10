examples = \
  seminar

all: $(examples:%=%.pdf)

%.pdf: %.tex
	pdflatex $(@:%.pdf=%.tex)
	pdflatex $(@:%.pdf=%.tex)

clean:
	rm -f *.aux *.log *.nav *.out *.snm *.toc *.vrb
