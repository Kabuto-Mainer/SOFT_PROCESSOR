#ifndef CONST_H
#define CONST_H

#define OFF 0
#define ON 1

#define PROCESSOR 0
#define DEBUG 1


#define MOD_WORK DEBUG

// Канареечная зашита
#define BIRD_SECURE ON

// Хеш зашиты
#define HASH_SECURE ON

//todo config.h
// Автоматическое увеличение стека
#define AUTO_REALLOC ON

// Заполнение стека
//* Необходимо для работы Канареечной и Хеш зашиты
#define COMPLETION_DATA ON


// Тип данных стека. В режиме процессора должен быть <int>
typedef int stmn_t;
const char name_type[] = "int";
#define PRINT_ELEMENT(num) { \
   printf("%d", num); \
}

// Количество регистров
const int AMOUNT_REGISTERS = 8;

// Множитель realloc
const int MOD_REALLOC = 2;
const int REALLOC_CONST = 5;

// Начальное количество команд
const size_t START_AMOUNT_CMD = 40;

const int START_SIZE_STACK = 4;

// Минимальное и максимальное значение аргументов
const stmn_t MAX_MEAN = 1e5;
const stmn_t MIN_MEAN = -1e5;

// Сид для хеш-функции
const size_t START_HASH_CODE = 5318;

// Ядовитое значение для стека
const stmn_t POISON_NUM = -3333;

// Минимальные рабочий адрес
const int MIN_ADDRESS = 8000;

// Количество элементов, которое выведется, если size > этого значения
const int AMOUNT_PRINT_ELEMENT = 10;

// Куропатка или Канарейка
const stmn_t BIRD_NUM = (stmn_t) 0xDEADFACE;

const int HACK_NUM = 10;

#endif // CONST_H
