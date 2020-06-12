# 2_sem
Этот репозиторий содежит задания из моего второго семестра обучения на ФРКТ. Далее следует краткий обзор некоторых задач.

## Бинарный транслятор

Транслятор из байт-кода моего процессора (`adasm`) в байт-код x86, с последующим созданием полноценного `.exe` файла.
____
В моём процессоре есть три команды, отсутствующие в каком-либо виде в ассемблере x86:

- `in`  -> считать число с клавиатуры и положить его в стек.
- `out` -> взять число из стека и вывести на экран.
- `end` -> завершить программу.

Эти команды требовалось заменить на соответствующие функции. Для `end` такая функция уже есть - `ExitProcess` из `kernel32.dll`.
Для `in` и `out` были написаны функции `ad6scanf` и `ToDec`, так как эти функции работают только с числами, а стандартным 
`scanf` и `printf`  требовалось бы передавать строку формата, что не очень удобно каждый раз.  
Чтобы пользоваться этими функциями через таблицу импорта, была создана динамическая библиотека [ad6lib.dll](https://github.com/derzhavin3016/2_sem/blob/master/bin_trans/ad6lib.dll).  

Для удобства подстановки команд x86 был создан файл [opcodes.h](https://github.com/derzhavin3016/2_sem/blob/master/bin_trans/opcodes.h).
____
Программе на вход поступает бинарный файл `.adasm`, полученный после работы моего процессора.
Для обработки команд передачи управления, совершающих переход по меткам (`jmp`, `jxx`, `call`)
была реализована структура `jmp_table`, хранящая информацию о всех переходах. Её элемент выглядит следующим образом:
```
struct jmp_inf         // structure for store information about jumps
{
  char *adasm_ptr;    // pointer to jmp command in adasm code
  char *x86_ptr;      // pointer to jmp command in x86 code
  char *adasm_dest;   // pointer to jump to in adasm code
  char *x86_dest;     // pointer to jump to in x86 code
};

```
Был заведён массив таких структур - таблица переходов:
```
struct jmp_table
{
  jmp_inf table[JMP_SIZE];
  size_t size;

  jmp_table( void );

  int JmpProcess( char *adasm_ptr, char *x86_ptr );

  bool CmdProcess( char *adasm_addr, char *x86_addr, bool IsJmp = false );

  bool PushOldInf( char *adasm_ptr, char *adasm_dest );
};
```
Для подсчёта относительного адреса перехода, была реализована двупроходная трансляция.
При первом проходе для каждой встреченной команды перехода создаётся новый элемент в таблице.
В нём заполняются поля `adasm_ptr` и `adasm_dest`.
Далее, при втором проходе перед каждой командой проверяется, является ли её адрес адресом 
назначения для какого-либо перехода, и если да, то в соответствующий(ие) элемент(ы) таблицы заносится `x86_dest` - указатель на команду-назанчение перехода в буфере кода.
Если же текущая команда является командой перехода, то дополнительно проверяется совпадение адреса текущей команды с адресом команды перехода из таблицы, затем в соответствующий элемент в поле
`x86_ptr` заносится вычисленный новый адрес команды перехода. Таким образом, данная таблица осуществляет взаимодействие между командами перехода и командами назначения, позволяя в любом порядке заносить относительный адрес назначения для команды перехода
(в случае, если сначала встречена команда перехода, то при попадании на команду нащначения, в таблице находится указатель на команду перехода, проходя по которому записывается относительный адрес перехода прямо в код; 
если, наоборот, сначала встречена команда назначения, то при попадании на соответствующую команду перезхода, заня из таблицы новый адрес команды, вычисляется относительное смещение и заносится после кода команды перехода).
____
После трансялции кода начинается создание `.exe` файла.
Первый заголовок `DOS_HEADER` заполняется по-умолчанию, также заполняется по умолчанию `DOS-STUB`(для этого была скопирована программа-заглушка из `.exe` файла для большей схожести с реальными файлами).
Далее идёт заполнение PE-заголовка и опционального заголовка, большинство полей заполняются нулями, либо значениями по-умолчанию.
При создании `.exe` файла используются 3 секции:
- `.data` -> секция данных (используется как замена `RAM` из процессора `adasm`
- `.text` -> секция кода (туда загружается бинарный код, полученный в результате трансляции)
- `.idata` -> таблица импорта (заполняется по умолчанию необходимим набором функций из [ad6lib.dll](https://github.com/derzhavin3016/2_sem/blob/master/bin_trans/ad6lib.dll).
____
Таким образом, создается готовый исполняемый файл.

Было проведено исследование эффективности бинарной трансяции, в котором считался факториал 100 раз.
Были получены следующие результаты:
|Время выполнения для `adasm`|Время выполнения оттранслированного `.exe` файла|Ускорение|
|:--:|:--:|:--:|
|14.2|0.3|47.7|

Получаем выигрыш почти в **50** раз.
