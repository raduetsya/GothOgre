#ifndef SYNONYM_PATH_H
#define SYNONYM_PATH_H

namespace GothOgre
{
	struct SynonymPath
	{
		/** Calculates a path which describes the same folder or file 
		as a specified source path,
		however a resulting path and a source path are different
		when they are compared as strings. */
		static String calculate(const String& _srcPath);
	};
}

#endif SYNONYM_PATH_H