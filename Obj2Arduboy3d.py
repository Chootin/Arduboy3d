#! /bin/python2

import sys
import re

vertexFormat = "{x:.4}f, {y:.4}f, {z:.4}f"

vertices = []
indices = []


def AddVertex(line):
    vertRe = re.search("^v (-?\d+.?\d*) (-?\d+.?\d*) (-?\d+.?\d*)", line)
    if vertRe:
        x = float(vertRe[1])
        y = float(vertRe[2])
        z = float(vertRe[3])
        vertices.append(vertexFormat.format(x = x, y = y, z = z))


def AddIndices(line):
    indicesRe = re.search("^f (\d+) (\d+) (\d+)", line)
    if indicesRe:
        indices.append(int(indicesRe[1]) - 1)
        indices.append(int(indicesRe[3]) - 1)
        indices.append(int(indicesRe[2]) - 1)
        

def SaveFile(inFileName):
    if len(vertices) > 255:
        raise ValueError("Found more than 255 vertices: {numVertices}!".format(numVertices = len(vertices)))

    fileRe = re.search("^(.+)\.obj$", inFileName)
                
    if fileRe:
        baseName = fileRe[1]
        baseName = baseName[0].lower() + baseName[1:]
        fileName = baseName[0].upper() + baseName[1:]
        
        outFileName = "{name}.h".format(name = fileName)
        print("Writing to: " + outFileName)
        
        #outString = "const Vector3f {baseName}Vertices[] = {{\n".format(baseName = baseName)
        outString = "const float {baseName}Vertices[] PROGMEM = {{\n".format(baseName = baseName)
        initialised = False
        
        for i in vertices:
            if initialised:
                outString += ",\n"
            
            outString += "  {vertex}".format(vertex = i)
            
            initialised = True
        
        outString += "\n};\n\n"
        
        initialised = False
        
        outString += "const uint8_t {baseName}Indices[] PROGMEM = {{\n".format(baseName = baseName)
        
        for i in range(0, len(indices), 3):
            v0 = indices[i]
            v1 = indices[i + 1]
            v2 = indices[i + 2]
            
            if initialised:
                outString += ",\n"
            
            outString += "  {v0}, {v1}, {v2}".format(v0 = v0, v1 = v1, v2 = v2)
            
            initialised = True
        
        outString += "\n};\n\n"
        
        outString += "const Mesh {baseName} = {{\n".format(baseName = baseName)
        outString += "  {numVertices}, {baseName}Vertices,\n  {numIndices}, {baseName}Indices".format(
            numVertices = len(vertices) * 3, baseName = baseName, numIndices = len(indices))
        
        outString += "\n};"
        
        outFile = open(r"{fileName}".format(fileName = outFileName), 'w')
        outFile.write(outString)
        outFile.close()


if __name__ == "__main__":
    if len(sys.argv) != 2 or not sys.argv[1].endswith(".obj"):
        raise ValueError("Usage: Model2Arduboy3d.py /path/to/obj.obj")

    with open(sys.argv[1]) as f:
        lines = f.readlines()
        for line in lines:
            if line[0] == "v":
                AddVertex(line)
            elif line[0] == "f":
                AddIndices(line)
                
        SaveFile(sys.argv[1])
            