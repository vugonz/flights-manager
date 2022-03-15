```
typedef struct {
	unsigned short day;
	unsigned short month;
	unsigned short year;
} data;

typedef struct {
	unsigned short hour;
	unsigned short minute;
} time;

typedef struct {
	char id[3];
	char country[30];
	char city[50];
} airport;

typedef struct {
	short n;
	short alph_id[2];
	airport destination;
	airport departure; 
	data data;
	time duration;
	short passenger;
} flight;
```
- `printf("%2d", data.member)` para ter sempre o output com dois dígitos, eg: `02` `05`


`printf("%2d-%2d-%2d-%2d-%2d", data.year, data.month, data.day, data.hour, data.minute);`
