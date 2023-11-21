#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// La: Macros in need
// La: offsetof macro 
#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
// La: container_of macro
#define container_of(ptr, type, member) ({   \
        const typeof(((type *)0)->member) *__mptr = (ptr); \
        (type *)((char *)__mptr - offsetof(type,member));})

struct list_head {
       struct list_head *next, *prev;
};

typedef struct car {
    struct list_head car_list;
    int max_speed;
    int price;
} car;

int main() {
    car car1;
    printf("Address of the container struct with normal method = %p\n", &car1);
    printf("Address of the first member = %p\n", &car1.car_list);
    printf("Address of the container struct according to the first member address = %p\n", container_of(&car1.car_list, struct car, car_list));
    printf("Address of the second member = %p\n", &car1.max_speed);
    printf("Address of the container struct according to the second member address = %p\n", container_of(&car1.max_speed, struct car, max_speed));
    
    return 0;     

}
