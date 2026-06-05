# System Regulacji Temperatury z wykorzystaniem STM32

## Opis projektu

Projekt przedstawia system regulacji temperatury oparty na mikrokontrolerze STM32F746. Układ mierzy temperaturę za pomocą czujnika BMP280 i steruje mocą elementu grzejnego przy użyciu sygnału PWM w celu utrzymania zadanej temperatury.

Aktualna temperatura, wartość zadana oraz sygnał sterujący są wyświetlane na wyświetlaczu LCD. Temperatura zadana może być zmieniana zarówno z poziomu klawiatury matrycowej, jak i poprzez interfejs UART.

## Funkcjonalności

* Pomiar temperatury przy użyciu czujnika BMP280 (interfejs I2C)
* Sterowanie elementem grzejnym za pomocą sygnału PWM
* Regulacja temperatury w pętli zamkniętej
* Wyświetlanie danych na wyświetlaczu LCD
* Ustawianie temperatury zadanej:

  * z klawiatury matrycowej,
  * przez komunikację UART
* Wysyłanie danych pomiarowych i sterujących przez port szeregowy

## Wykorzystane elementy

* Mikrokontroler STM32F746
* Czujnik BMP280
* Wyświetlacz LCD
* Klawiatura matrycowa
* Element wykonawczy sterowany PWM (np. grzałka)

## Oprogramowanie

* STM32CubeIDE
* Biblioteki STM32 HAL
* Sterownik BMP280
* Biblioteka LiquidCrystal

## Zasada działania

Program cyklicznie odczytuje temperaturę z czujnika BMP280, a następnie wyznacza sygnał sterujący na podstawie różnicy pomiędzy temperaturą zmierzoną a temperaturą zadaną. Wyliczony sygnał jest przekształcany na współczynnik wypełnienia PWM, który steruje mocą elementu grzejnego.

Aktualne wartości temperatury, temperatura zadana oraz sygnał sterujący są prezentowane na wyświetlaczu LCD oraz przesyłane przez interfejs UART.

## Możliwości rozbudowy

* Rejestracja danych pomiarowych na karcie SD
* Wizualizacja danych na komputerze
* Sterowanie przez Ethernet lub Wi-Fi
* Implementacja bardziej zaawansowanych regulatorów (PI, PID)
* Obsługa alarmów przekroczenia temperatury

## Autor

Projekt wykonany w środowisku STM32CubeIDE z wykorzystaniem biblioteki HAL dla mikrokontrolerów STM32.

