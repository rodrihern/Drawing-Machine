#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int main () {
	
	char str[] = "G01 X0.1 Y50.22";
	float nuevoY, nuevoX;
	
	puts(str);
	
	char* indexX = strchr( str, 'X' );  // Get X/Y position in the string (if any)
    char* indexY = strchr( str, 'Y' );
    
    printf("%c %c \n\n", *indexX, *indexY);
    
    nuevoY = atof(indexY + 1);
    *indexY = '\0'; 						// No se para que esta esta linea y a veces me genera problemas en el .ino
    nuevoX = atof(indexX + 1);
    
    printf("%.2f \n%.2f", nuevoX, nuevoY);
    
    
	
	
}


