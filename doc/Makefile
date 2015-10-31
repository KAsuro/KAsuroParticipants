.PRECIOUS: de en
.PHONY: all clean distclean de pdf_de en pdf_en

all: pdf_de pdf_en

clean: distclean
	rm -rf de/html
	rm -rf en/html
	rm -f de/dokumentation.pdf
	rm -f en/documentation.pdf

distclean:
	rm -rf de/latex
	rm -rf en/latex

pdf_de: de
	mv $</latex/refman.pdf $</dokumentation.pdf

pdf_en: en
	mv $</latex/refman.pdf $</documentation.pdf

de en:
	doxygen doc_$@
	$(MAKE) -C $@/latex all
