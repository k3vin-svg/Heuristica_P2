#!/usr/bin/env python3

import sys
import os
import subprocess

def main():
    # comprobamos que nos pasen el archivo de entrada y el de salida
    if len(sys.argv) != 3:
        print(
            "Uso: ./sol-2.py <fichero_entrada> <fichero_salida>",
            file=sys.stderr
        )
        sys.exit(1)

    carpeta = os.path.dirname(os.path.abspath(__file__))

    entrada = sys.argv[1]
    salida = sys.argv[2]

    # los nombres sin carpeta se interpretan junto al script
    if not os.path.dirname(entrada):
        entrada = os.path.join(carpeta, entrada)

    if not os.path.dirname(salida):
        salida = os.path.join(carpeta, salida)

    entrada = os.path.abspath(entrada)
    salida = os.path.abspath(salida)

    # make recompila los archivos que hayan cambiado
    try:
        subprocess.run(
            ["make"],
            cwd=carpeta,
            check=True,
            stdout=subprocess.DEVNULL,
            stderr=subprocess.PIPE,
            text=True
        )

    except subprocess.CalledProcessError as error:
        print("Error al compilar C++:", file=sys.stderr)
        print(error.stderr, end="", file=sys.stderr)
        sys.exit(1)

    except OSError as error:
        print(f"No se ha podido ejecutar make: {error}", file=sys.stderr)
        sys.exit(1)

    # pasamos los dos archivos al programa principal
    programa = os.path.join(carpeta, "principal")
    comando = [programa, entrada, salida]

    try:
        # recogemos la salida del programa para mostrarla por pantalla
        proceso = subprocess.run(
            comando,
            text=True,
            capture_output=True
        )

        print(proceso.stdout, end="")

        if proceso.stderr:
            print(proceso.stderr, end="", file=sys.stderr)

        sys.exit(proceso.returncode)

    except OSError as error:
        print(
            f"No se ha podido ejecutar el programa: {error}",
            file=sys.stderr
        )
        sys.exit(1)

if __name__ == "__main__":
    main()