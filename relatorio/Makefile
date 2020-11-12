INSTALL=sudo apt-get install
LATEX=texlive-latex-base texlive-fonts-recommended texlive-fonts-extra texlive-latex-extra texlive-publishers texlive-humanities

all: relatorio

relatorio:

	@$(INSTALL) $(LATEX)
	@(cd IEEEtran && pdflatex relatorio.tex >/dev/null)
	@(cd IEEEtran && mv relatorio.pdf ../relatorio)

clean: 
	$(RM) relatorio
