all: presentation.tex
	pdflatex presentation.tex

open : all
	evince presentation.pdf

images: all
	convert -resize 800x600 -quality 100 -density 400 presentation.pdf presentation_jpg/presentation.jpg
	mv presentation_jpg/presentation-0.jpg presentation_jpg/presentation-00.jpg
	mv presentation_jpg/presentation-1.jpg presentation_jpg/presentation-01.jpg
	mv presentation_jpg/presentation-2.jpg presentation_jpg/presentation-02.jpg
	mv presentation_jpg/presentation-3.jpg presentation_jpg/presentation-03.jpg
	mv presentation_jpg/presentation-4.jpg presentation_jpg/presentation-04.jpg
	mv presentation_jpg/presentation-5.jpg presentation_jpg/presentation-05.jpg
	mv presentation_jpg/presentation-6.jpg presentation_jpg/presentation-06.jpg
	mv presentation_jpg/presentation-7.jpg presentation_jpg/presentation-07.jpg
	mv presentation_jpg/presentation-8.jpg presentation_jpg/presentation-08.jpg
	mv presentation_jpg/presentation-9.jpg presentation_jpg/presentation-09.jpg

clean:
	rm *.aux *.log *.nav *.out *.pdf *.snm *.toc
