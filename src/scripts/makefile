# Variáveis de Compilação
CC = gcc
CFLAGS = -Wall -g
LIBS = -lgmp

# Regra Genérica para Compilar e Ligar:
# Quando fazemos 'make nome_do_programa', esta regra é usada.
# O make vai tentar criar um executável com o nome do ALVO ($@).
# Assume que existe um ficheiro C correspondente ($@.c).
%: %.c
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

