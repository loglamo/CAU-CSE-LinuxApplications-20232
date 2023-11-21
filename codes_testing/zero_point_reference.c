#include <stdio.h>

struct list_head {
       struct list_head *next, *prev;
};

struct car {
       struct list_head car_list;
       int max_speed;
       int price;
};

int main(void) {
    // struct car car1;
    printf("Offset in byte of car_list member = %ld\n", (size_t)&((struct car*)0)->car_list);
    printf("Offset in byte of max_sp member = %ld\n", (size_t)&((struct car*)0)->max_speed);
    printf("Offset in byte of price member = %ld\n", (size_t)&((struct car*)0)->price);
    return 0;
}