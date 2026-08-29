# obligatorio1 2026 s2

Primer obligatorio de la materia **Estructuras de Datos y Algoritmos 1**, Facultad de Ingeniería, ORT Uruguay.

## Archivos a completar

El obligatorio tiene _stubs_ que deben ser completados por el o la estudiante, en la carpeta [src/ejercicios](./src/ejercicios/). Cada archivo tiene su propia entrega.

En concreto, hay que reemplazar el texto:

```cpp
// IMPLEMENTAR SOLUCION
throw NoImplementado;
```

por la solución.

* [comienzo.cpp](./src/ejercicios/comienzo.cpp): ejercicios básicos de C y de ordenación.
* [listas.cpp](./src/ejercicios/listas.cpp): ejercicios de listas simplemente enlazadas.
* [arboles.cpp](./src/ejercicios/arboles.cpp): ejercicios de árboles binarios y árboles generales.

Cada archivo de ejercicios tiene su definición en el directorio [include/ejercicios](./include/ejercicios/), donde en el comentario precediendo la declaración de cada función se encuentra la letra detallando lo esperado.


> [!CAUTION]
> Cada entrega incluye únicamente uno de los archivos indicados. Cada
> archivo debe contener todas las funciones necesarias, no se puede usar
> `#include` (salvo los ya colocados).

## Uso de IA

Las defensas del obligatorio (una por cada entrega) y los parciales son en papel, con lo que el objetivo de estos ejercicios es que puedan adquirir y reproducir los conocimientos. Si la IA genera la respuesta, entonces la posibilidad de que haya adquirido el conocimiento es baja, y redundará en un bajo puntaje. Por ello, su uso debe ser meramente consultivo. Por ejemplo, en vez del siguiente prompt:

> Resuelve el ejercicio tablaDel

Escribir prompts del estilo:

> ¿Cómo hago para iterar desde `desde` hasta `hasta` en C?

## Requisitos

Para compilar el proyecto es necesario contar con:
 * [CMake](https://cmake.org/)
 * Compilador de C++
 * (Opcional pero deseable) Un IDE moderno como Visual Studio Code.

En [el siguiente link](https://code.visualstudio.com/docs/cpp/config-msvc#cpp-articles) pueden encontrar artículos de cómo instalar las distintas herramientas según su plataforma.

> [!CAUTION]
> Si el archivo que entregan no compila en una versión fresca del proyecto
> entonces no será tenido en cuenta en la corrección y obtendrá 0 puntos.


## Instalación recomendada en Windows con Visual Studio Code

En [WINDOWS.md](WINDOWS.md) están las instrucciones para poder compilar el proyecto en VSCode, centrado en Windows
pero muchos de los pasos deberían funcionar también en otras plataformas.

## Instalación recomendada en macOS con Visual Studio Code

En [MACOS.md](MACOS.md) están las instrucciones para poder compilar el proyecto en VSCode para macOS.

### Alternativas

#### Compilación con CMake

Desde la raíz del proyecto, configurar y compilar, ejecutando en una terminal:

```bash
cmake -S . -B build
cmake --build build
```

El ejecutable queda disponible en `build/Debug/obligatorio1.exe` (o `build/obligatorio1` en Linux o macOS). Para ejecutarlo, escribir en la terminal (o el equivalente en otra plataforma):

```bash
./build/Debug/obligatorio1.exe
```

Para recompilar luego de modificar archivos, basta con ejecutar nuevamente:

```bash
cmake --build build
```

#### Compilación con Docker

El `Dockerfile` incluye CMake, un compilador de C++ y `make` como herramienta de compilación. Primero, crear la imagen desde la raíz del proyecto:

```bash
docker buildx build --load -t obligatorio1 .
```

La opción `--load` deja la imagen construida disponible para los comandos `docker run`. Si Docker informa que `buildx` no está instalado, se debe instalar o habilitar ese componente de Docker antes de ejecutar el comando.

Luego, montar el proyecto en el contenedor y compilarlo. Se usa `build-docker` para no mezclar la caché de CMake del contenedor con una compilación local:

```bash
docker run --rm -v "$PWD":/workspace obligatorio1 \
  cmake -S . -B build-docker
docker run --rm -v "$PWD":/workspace obligatorio1 \
  cmake --build build-docker
```

Para ejecutar las pruebas desde el contenedor:

```bash
docker run --rm -v "$PWD":/workspace obligatorio1 \
  ./build-docker/obligatorio1
```

También se puede abrir una terminal dentro del contenedor por si se quiere ejecutar los comandos directamente:

```bash
docker run --rm -it -v "$PWD":/workspace obligatorio1
```

#### Consideración según plataforma

Estos comandos funcionan en macOS y Linux con una terminal compatible con Bash o Zsh. En Windows se recomienda usar Docker Desktop y PowerShell; para montar el directorio actual, usar `${PWD}`:

```powershell
docker buildx build --load -t obligatorio1 .
docker run --rm -v "${PWD}:/workspace" obligatorio1 cmake -S . -B build-docker
docker run --rm -v "${PWD}:/workspace" obligatorio1 cmake --build build-docker
docker run --rm -v "${PWD}:/workspace" obligatorio1 ./build-docker/obligatorio1
```

En Windows y macOS, Docker Desktop normalmente administra los permisos y Buildx. En Linux, si aparece un error de permiso para `/var/run/docker.sock`, ejecutar Docker con `sudo` o configurar el acceso al grupo `docker` según la instalación local.

## Ejecutar pruebas con Catch2

El ejecutable usa Catch2. Sin argumentos ejecuta todas las pruebas:

```bash
./build/Debug/obligatorio1.exe
```

Para ejecutar una prueba específica, indicar su etiqueta o su nombre. Por ejemplo:

```bash
./build/Debug/obligatorio1.exe "[PruebaSuma]"
```

Para ejecutar todas las pruebas asociadas a un archivo, usar la etiqueta `file`:

```bash
./build/Debug/obligatorio1.exe "[file:listas]"
./build/Debug/obligatorio1.exe "[file:arboles]"
./build/Debug/obligatorio1.exe "[file:comienzo]"
```

Por defecto se usa el reporte de consola. Para una salida compacta, agregar la opción `--reporter`:

```bash
./build/Debug/obligatorio1.exe --reporter compact
```

Para obtener únicamente el porcentaje de casos de prueba aprobados, usar el
reporte `grading`:

```bash
./build/Debug/obligatorio1.exe "[file:listas]" --reporter grading
```
