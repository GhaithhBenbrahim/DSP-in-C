#include <stdio.h>
#include <stdlib.h>

int main() {
    char *s = (char *)malloc(100 * sizeof(char));
    if (s == NULL) {
        printf("Memory allocation failed\n");
        return 1;
    }

    printf("Enter your name: ");
    scanf("%99s", s);  // Read up to 99 characters to avoid overflow
    printf("Hello, %s\n", s);

    free(s);  // free allocated memory
    return 0;
}
