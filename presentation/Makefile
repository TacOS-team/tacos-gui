all: presentation.tex
	pdflatex presentation.tex

open : all
	evince presentation.pdf

images: all
	convert -resize 800x600 -quality 100 -density 400 presentation.pdf presentation_jpg/presentation.jpg

clean:
	rm *.aux *.log *.nav *.out *.pdf *.snm *.toc
