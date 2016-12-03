#pragma once
#define MAX_BUFFER 4096
namespace MindSpy
{
	enum TIPO_DATO {
		NULO,
		MENSAJE=0,
		FICHEROS,
		SYSINFO
	};

	typedef struct stFichero
	{
		char Archivos[MAX_BUFFER];
		char Directorios[MAX_BUFFER];
	} Fichero, *PFichero;

	class Datos {
	public:
		TIPO_DATO Tipo;
		char *data;
		Datos(TIPO_DATO TipoDato, char*buff) : Tipo(TipoDato), data(buff) {};
		Datos() : Tipo(NULO), data(NULL) {};
	};
}