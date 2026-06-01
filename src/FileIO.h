#ifndef FILEIO_H
#define FILEIO_H

#include "Warehouse.h"
#include <string>

class FileIO {
public:
    static bool load(const std::string& filename, Warehouse& warehouse);
    static bool save(const std::string& filename, const Warehouse& warehouse);
};

#endif // FILEIO_H