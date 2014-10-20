# need to copy LESS-processed css back here to push to github
build:
	jekyll build
	cp -r _site/css ./

code:
	$(MAKE) -C _includes

serve:
	jekyll serve --watch --baseurl=
