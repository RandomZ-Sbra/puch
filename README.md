---
runme:
  id: 01HTSRCDVD1MFW4F5NGRW6H8PH
  version: v3
---

# Puch

---

## Version: 0.2

Esta versión está incompleta, la primera versión funcional será la 1.0; durante las versiones 0.x se desarrollan conceptos, programas e ideas para versiones siguientes.

### TODO:

- Lista de cosas por hacer (todavía no implementada)

---

## Índice

- [Descripción](#descripción)
- [Entorno](#prepare-el-entorno)
- [Uso](#uso-del-programa)

## Descripción

**Puch** es un programa básico de mensajería privada a través del protocolo TCP. El programa cifra los mensajes de forma simétrica con el uso de libretas desechables, tenga en cuenta que para que el cifrado sea efectivo las libretas deben pasarse de forma privada entre usuarios (lo ideal sería en persona y a través de compartir un medio físico como un USB o por transferencia de cable).

De momento el programa está disponible únicamente en dispositivos Unix, más concretamente para Ubuntu. Si dispone de una máquina windows use [Windows Subsystem for Linux](https://apps.microsoft.com/detail/9pdxgncfsczv?hl=en-us&gl=US) o "*WSL*" con Ubuntu preferiblemente.

## Prepare el entorno

Simplemente descargue el fichero Puch.c y compile en su máquina para usarlo. Ver notas de [despliegue](#Despliegue) para más información.

En el entorno se puede especificar la variable **PUCH_PATH** para personalizar el directorio destino donde se generarán los contactos y logs de la aplicación. Por defecto, si no se especifica, esta se encuentra en **``/var/lib/puch-messages-v``\[[NÚMERO_DE_VERSIÓN](#version-02)\]** (se debe ejecutar el programa con los permisos necesarios para acceder a la carpeta indicada).

### Prerrequisitos

- Un sistema linux con terminal.
- Compilador de c como gcc (opcional en futuras versiones donde se pretende incluir el archivo ejecutable).
- Acceso a internet.
- Al menos un *Trusted Server* para establecer la comunicación entre clientes (opcional pero muy recomendable).

### Instalación

Descargue el fichero puch.c y genere el ejecutable con gcc:

```sh {"id":"01HTST9CP9C9PFA0D63VJX4R3R"}
gcc puch.c -o puch
```

Inicialice la aplicación (en futuras versiones se pretende añadir opciones y argumentos...):

```sh {"id":"01HTSRCDVD1MFW4F5NGKPD7KQJ"}
./puch
```

Opcionalmente se puede especificar el directorio de destino declaranto la variable de entorno ``PUCH_PATH``:

```sh {"id":"01HTSTEPE8G5ZH7MCGKPTF546Q"}
env PUCH_PATH="/home/usuario/miDirectorio/directorioPuch" ./puch
```

En el directorio de ``PUCH_PATH`` se generarán dos subdirectorios: ``$PUCH_PATH/contactos`` y ``$PUCH_PATH/logs``.

## Uso del programa

Todavía en desarrollo...
