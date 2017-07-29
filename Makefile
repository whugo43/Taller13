pc: productor_consumidor.c
	gcc -Wall productor_consumidor.c -o pc -pthread
clean:
	rm -rf pc
