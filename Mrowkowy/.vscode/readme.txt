Jak dobrze skompilować i debugować C++/C  w VS Code - tutorial by Tomasz P.	10.10.2020

inspirowane filmem z youtube ale zmienione .vscode żeby wszystko ładnie śmigało
https://www.youtube.com/watch?v=ABVeAXcRIJg&ab_channel=Codeflash

linki do potrzebnych rzeczy:
https://code.visualstudio.com/
https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools
http://mingw-w64.org/doku.php/download/mingw-builds

0. Jak wiesz że namieszałeś/aś wcześniej to odinstaluj wszystko- VS Code, namieszane rzeczy w PATH i dodatki do VS Code
0.5. Otwieraj VS Code ZAWSZE jako ADMINISTRATOR.
1. Zainstalować MinGW w taki sposób, żeby folder bin znajdował się pod adresem "C:\\MinGW\\bin"
2. Dodać C:/MinGW/bin do zmiennych środowiskowych usera
3. Zainstalować VS Code
4. Zainstalować dodatek do VS Code: C/C++ IntelliSense
4. Utworzyć workspace i wkleić gotowe configuracje .vscode (cały folder)
5. Jak już mamy program jakistam.cpp to budujemy go w zakladce: terminal --> build and run
6. Utworzy sie jakistam.exe i juz normalnie mozemy go odpalić albo debugować w zakładce run