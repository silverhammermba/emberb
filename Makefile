# need to copy LESS-processed css back here to push to github
build:
	jekyll build
	cp -r _site/css ./

test:
	$(MAKE) -C _test

serve:
	jekyll serve --watch --baseurl=
