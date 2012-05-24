all: build diff

# Vyroba PDF primo z DVI by byla prijemnejsi, ale vetsina verzi dvipdfm nici obrazky
# prace.pdf: prace.dvi
#	dvipdfm -o $@ -p a4 -r 600 $<

.PHONY: build diff run run-diff clean

BUILD_DIR=build
DIFF_DIR=diff

ifndef diff_commit
diff_commit=HEAD^
endif

prace.pdf: ${BUILD_DIR}/prace.pdf
	cp $< $@

${BUILD_DIR}/prace.pdf: prace.tex $(wildcard *.tex) $(wildcard img/*.pdf) $(wildcard img/*.tex)
	mkdir -p ${BUILD_DIR}
	pdfcslatex -output-directory=${BUILD_DIR} $<
	pdfcslatex -output-directory=${BUILD_DIR} $<

diff.prace.pdf: ${DIFF_DIR}/prace.pdf
	cp $< $@

${DIFF_DIR}/prace.pdf: ${DIFF_DIR}/prace.tex $(wildcard ${DIFF_DIR}/*.tex) $(wildcard img/*.pdf)
	pdfcslatex -output-directory=${DIFF_DIR} $< 
	pdfcslatex -output-directory=${DIFF_DIR} $< 

${DIFF_DIR}/prace.tex: prace.tex $(wildcard *.tex)
	mkdir -p ${DIFF_DIR}
	for i in *.tex; do \
		if [ "$$i" = "literatura.tex" ]; then continue; fi; \
		git ldiff ${diff_commit} $$i > ${DIFF_DIR}/$$i; \
	done
#latexdiff prace.tex prace.tex | sed -e 's/include{\([^{}]\+\)}/include{${DIFF_DIR}\/\1}/' > ${DIFF_DIR}/prace.tex
	cp literatura.tex ${DIFF_DIR}/literatura.tex
	latexdiff prace.tex prace.tex | sed -e 's/\\usepackage{encxvlna}//' > ${DIFF_DIR}/prace.tex

build: prace.pdf
diff: diff.prace.pdf

run: build
	evince prace.pdf &

run-diff: diff
	evince diff.prace.pdf &

clean:
	rm -f *.{log,dvi,aux,toc,lof,out} prace.ps prace.pdf diff.prace.pdf
	rm -rf build
	rm -rf diff
