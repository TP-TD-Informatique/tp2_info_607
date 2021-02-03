##
## CONSIGNES :
##    - votre fichier principal devra s'appeler "prenom-nom.c" ou
##      "nom1-nom2.c" si vous êtes en binômes
##    - il faut modifier la variable "NOM" ci dessous pour qu'elle corresponde
##      au nom du fichier (sans le ".c")
##    - pour compiler, il suffit de faire
##      $ make
##      dans le répertoire qui contient les fichiers
##


################
## à modifier ##
################
NOM = geyer-traini-finck

# FLAGS = -Wall -std=c99 -Wextra -pedantic -Werror
FLAGS = -Wall -std=c99 -Wextra -pedantic -Werror -Wno-unused-parameter -Wno-unused-function



#----------------------------------------------------------------------
#----- NE RIEN MODIFIER SOUS CETTE LIGNE ------------------------------
#----------------------------------------------------------------------

FILES_C = utils.c lcg.c lfsr.c main.c $(NOM).c tests-$(NOM).c
FILES_O = $(FILES_C:.c=.o)

CC = gcc $(FLAGS)
# CC = clang $(FLAGS)

all: $(NOM)

$(NOM): $(FILES_O)
	$(CC) $(FILES_O) -o $(NOM)

albus-dumbledore.o: geyer-traini-finck.c tp2.h
ifeq ($(USER),hyvernat)
	$(CC) -c -o $@ $<
else
ifeq ($(USER),pierre)
	$(CC) -c -o $@ $<
else
	@echo
	@echo "CHANGEZ LE NOM DU FICHIER $< ET"
	@echo "MODIFIEZ LA VARIABLE 'NOM' DANS LE FICHIER Makefile"
	@echo
	@false
endif
endif

%.o: %.c tp2.h
	$(CC) -c -o $@ $<

clean:
	rm -f $(NOM) *.o a.out correction albus-dumbledore

