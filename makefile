control: control.c
	gcc control.c -o control

write: write.c
	gcc write.c -o write

clean:
	rm *~