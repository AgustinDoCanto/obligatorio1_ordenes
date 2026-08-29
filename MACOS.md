# Instrucciones para usar VSCode en macOS
 
### Developer tools

Abrir la terminal y escribir `gcc` y enter. Si retorna algo del estilo que el comando espera archivos, entonces ya está instalado. Caso contrario, debería sugerir instalar los Developer tools, y simplemente decir que sí quieren instalarlo. Una vez instalado debería reconocer el comando `gcc` y `git`.

### VScode 

Descargar e instalar [Visual Studio Code](https://code.visualstudio.com/).

### CMake

Es necesario instalar CMake. Para esto, se puede utilizar `brew` si ya lo tienen instalado:

```sh
brew install cmake
```

O bajar el .dmg de la página de descargas de [CMake](https://cmake.org/download) correspondiente a su sistema, instalarlo como cualquier app de macOS, y ejecutarlo. Una vez abierto, debería haber una opción `Tools -> How to install for command line use` que da distintas opciones para instalarlo. Se sugiere la que instala en el `/usr/local/bin` (mediante acceso de privilegio con `sudo`).

### Repositorio

Para obtener el código del obligatorio, abrir terminal en alguna carpeta a elección, y ejecutar:

```sh
git clone https://github.com/Algoritmos1-ORT/obligatorio1_v2.git
```

Para abrirlo en VSCode navegar al directorio y abrir el VSCode:

```sh
cd ./obligatorio1_v2/
opan -A "Visual Studio Code" .
```

### Compilación

Cuando se abre el proyecto, hay que marcar la carpeta como "Trusted" (Confiable?). Luego, ir a las extensiones,
buscar e instalar `C/C++ Extension Pack`.

Una vez instalados, abrir la paleta de comandos (`Ctrl + Shift + P`) y escribir `CMake scan for kit`. Esto
busca todos los compiladores de C/C++ que tengan instalados.

Abrir la paleta de comandos de nuevo (`Ctrl + Shift + P`) y escribir `CMake select a kit`. De la lista se puede
elegir `Unspecified`.

Una vez elegido, abajo a la izquierda aparece el botón de compilar (build), el de debuggear, y el play de ejecutar:

![VSCode tools para CMake](./assets/images/vscode.png)
