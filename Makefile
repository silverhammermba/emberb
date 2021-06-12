serve:
	bundle exec jekyll serve --watch --baseurl=

build:
	bundle exec jekyll build

test:
	$(MAKE) -C _test
