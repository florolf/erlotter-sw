all: config.bin production.db test.bin
	./make-image.sh

config.bin: ../default-timeline
	perl build-config.pl --timeline=../default-timeline > config.bin

production.db: production_schema.sql
	sqlite3 $@ < $^

test.bin: ../../firmware/build/test/firmware.bin
	cp $^ $@

clean:
	rm -f *.bin

mrproper: clean
	rm -f production.db
