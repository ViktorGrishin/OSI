all: build prepare_links

build:
	gcc main.c -o task2

prepare_links: build 
	mkdir -p lnprograms
	cd lnprograms && \
	ln -f ../task2 create_dir && \
	ln -f ../task2 read_dir && \
	ln -f ../task2 create_file && \
	ln -f ../task2 delete_file && \
	ln -f ../task2 create_symlink && \
	ln -f ../task2 read_symlink_target && \
	ln -f ../task2 read_symlink && \
	ln -f ../task2 delete_symlink && \
	ln -f ../task2 create_hardlink && \
	ln -f ../task2 delete_hardlink && \
	ln -f ../task2 stat_file && \
	ln -f ../task2 chmod_file && \
	ln -f ../task2 delete_dir && \
	ln -f ../task2 create_dir

clean:
	rm -rf lnprograms
	rm -f task2
