# need to copy LESS-processed css back here to push to github
build:
	jekyll build
	cp -r _site/css ./

serve:
	jekyll serve --watch --baseurl=
