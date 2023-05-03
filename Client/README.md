# Клиентская программа

Посылка команд на сервер. Сервер крутится на плате.

## Использование
`./client --command <команда>`

`./client --help`

## Доступне команды
* 11 &ndash; зажечь LD1
* 10 &ndash; потушить LD1
* 21 &ndash; зажечь LD2
* 20 &ndash; потушить LD2
* 31 &ndash; зажечь LD3
* 30 &ndash; потушить LD3

## Сборка
`clang++ client.cxx -o client --std=c++17 -lboost_program_options`
