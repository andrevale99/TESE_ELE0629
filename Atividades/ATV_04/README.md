| Supported Targets | ESP32 | ESP32-C2 | ESP32-C3 | ESP32-C6 | ESP32-H2 | ESP32-P4 | ESP32-S2 | ESP32-S3 |
| ----------------- | ----- | -------- | -------- | -------- | -------- | -------- | -------- | -------- |

# Atividade 04

A ideia é que vocês pensem em um "projeto" simples que:

* Utilize dois periféricos ainda não utilizados;

* Periféricos já utilizados (Timer, PWM, ADC) podem ser utilizados como auxílio;

* O programa deve ser baseado no FreeRTOS (tasks, queue, mutex e semáforos);


Caso não tenham um projeto em mente, vocês desenvolverão um multimetro true RMS:

* Ser capaz de ler um sinal senoidal com amplitude variando entre 0 e 10 V e frequência até 50 kHz;

* Fazer o calculo do valor RMS;

* Fazer o calculo do valor true RMS;

* Disponibilidar esses valores em algum display.

A atividade pode ser realizada em dupla e deve ser implementada no esp32 com sensores, circuitos de condicionamento, etc e não apenas por simulação.

Enviar a pasta do projeto compactada com um arquivo explicando o desenvolvimento do projeto (ideia + circuito + código).

<!--================================================-->
## Example folder contents

The project **sample_project** contains one source file in C language [main.c](main/main.c). The file is located in folder [main](main).

ESP-IDF projects are built using CMake. The project build configuration is contained in `CMakeLists.txt`
files that provide set of directives and instructions describing the project's source files and targets
(executable, library, or both). 

Below is short explanation of remaining files in the project folder.

```
├── CMakeLists.txt
├── main
│   ├── CMakeLists.txt
│   └── main.c
└── README.md                  This is the file you are currently reading
```
Additionally, the sample project contains Makefile and component.mk files, used for the legacy Make based build system. 
They are not used or needed when building with CMake and idf.py.
