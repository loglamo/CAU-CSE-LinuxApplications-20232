#include "calclock.h"

/**
 * @brief make number as separated with commas
 *
 * @number input number
 * @buffer number string buffer
 * @return separated number string buffer itself
 */
static const char *separate_num(unsigned long long number, char buffer[])
{
	char tmp_buff[100]; // temp buffer for characterized numbers
	char tmp_reverse_buff[100]; // temp buffer for saving reversed numbers
	int cur, counter = 0, rvs_cur = 0;

	sprintf(tmp_buff, "%llu", number);
	cur = strlen(tmp_buff);

	for (--cur; cur > -1; cur--) {
		if (counter == 3) {
			tmp_reverse_buff[rvs_cur++] = ',';
			counter = 0;
			cur++;
		}
		else {
			tmp_reverse_buff[rvs_cur++] = tmp_buff[cur];
			counter++;
		}
	}

	cur = 0;
	for (--rvs_cur; rvs_cur > -1; rvs_cur--) {
		buffer[cur++] = tmp_reverse_buff[rvs_cur];
	}

	buffer[cur] = '\0'; // inserting null char

	return buffer;
}

void __ktprint(int depth, char *func_name, ktime_t time, unsigned long long count)
{
	char char_buff[100], char_buff2[100]; // buffer for characterized numbers
	int percentage;
	static ktime_t totaltime = 1;

	if (ktime_before(totaltime, time))
		totaltime = time;
	percentage = time * 10000 / totaltime;

	printk("%s", "");

	while(depth--)
		printk(KERN_CONT "    ");
	printk(KERN_CONT "%s is called ", func_name);
	printk(KERN_CONT "%s times, ", separate_num(count, char_buff));
	printk(KERN_CONT "and the time interval is %sns (per thread is %sns)", 
			separate_num((u64)ktime_to_ns(time), char_buff), 
			separate_num((u64)(ktime_to_ns(time) / num_online_cpus()), char_buff2));
	printk(KERN_CONT " (%d.%.2d%%)\n", percentage/100, percentage%100);
}

