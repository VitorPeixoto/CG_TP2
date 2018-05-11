#include "Mesh.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <math.h>

using namespace std;

const int U = 0;
const int V = 1;

const int X = 0;
const int Y = 1;
const int Z = 2;
const int W = 3;

class bar_ctype : public ctype<char> {
public:
    mask const *get_table() {
        static vector<ctype<char>::mask>
                table(classic_table(), classic_table()+table_size);
        table['/'] = (mask)space;
        return &table[0];
    }
    bar_ctype(size_t refs=0) : ctype<char>(get_table(), false, refs) { }
};

Mesh::~Mesh() {
    if (verticesVBO != 0)
      glDeleteBuffers(1, &verticesVBO);
    if (normalsVBO != 0)
      glDeleteBuffers(1, &normalsVBO);
    if (elementsIBO != 0)
      glDeleteBuffers(1, &elementsIBO);
}

/**
 * Faz o upload dos vetores para os buffers
 **/
void Mesh::upload() {
    if (this->vertices.size() > 0) {
        // Gera um buffer para ser salvo no endere�o de verticesVBO
        glGenBuffers(1, &verticesVBO);
        // Associa o id/nome(GLuint) verticeVBuffer ao alvo (GL_ARRAY_BUFFER)
        glBindBuffer(GL_ARRAY_BUFFER, this->verticesVBO);
        // Cria um local de armazenamento com o tamanho definido, com o alvo, e passando os dados do vetor.
        // O �ltimo parametro define a natureza de acesso e modifica��o dos valores armazenados
        // Ex.: Voc� sobe os valores e n�o altera, modifica somente usando translate e scale no mundo
        // antes de desenhar
        glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(this->vertices[0]),
                     this->vertices.data(), GL_STATIC_DRAW);
    }

    if (this->normals.size() > 0) {
        glGenBuffers(1, &this->normalsVBO);
        glBindBuffer(GL_ARRAY_BUFFER, this->normalsVBO);
        glBufferData(GL_ARRAY_BUFFER, this->normals.size() * sizeof(this->normals[0]),
                     this->normals.data(), GL_STATIC_DRAW);
    }

    if (this->elements.size() > 0) {
        glGenBuffers(1, &this->elementsIBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->elementsIBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->elements.size() * sizeof(this->elements[0]),
                     this->elements.data(), GL_STATIC_DRAW);
    }
}

void Mesh::loadFromFile(const char* fileName) {
    // abre o arquivo para leitura
    ifstream in(fileName, ios::in);
    if (!in) {
        cerr << "Erro ao abrir o arquivo" << fileName << endl;
        exit(1);
    }

    string line;
    while (getline(in, line)) {
        if (line.substr(0, 2) == "v ") { // Linha de v�rtice
            // stream de string para facilitar a vida
            istringstream vertexLine(line.substr(2));

            Vector4d vertex;
            vertexLine >> vertex[X];
            vertexLine >> vertex[Y];
            vertexLine >> vertex[Z];
            vertex[W] = 1.0; //Homogenea ponto

            this->vertices.push_back(vertex);
        } else if (line.substr(0, 2) == "f ") { // Linha de face
            istringstream faceLine(line.substr(2));
            faceLine.imbue(locale(locale(), new bar_ctype));
            Triangle triangle;

            faceLine >> triangle.faces[0].vertex >> triangle.faces[0].texture >> triangle.faces[0].normal;
            faceLine >> triangle.faces[1].vertex >> triangle.faces[1].texture >> triangle.faces[1].normal;
            faceLine >> triangle.faces[2].vertex >> triangle.faces[2].texture >> triangle.faces[2].normal;

            // Vertices no arquivo: 1:n, transforma para 0:(n-1)
            for (auto &face : triangle.faces) { face.decrease(); }

            this->elements.push_back(triangle);
        } else if (line.substr(0, 2) == "vt") {
            // stream de string para facilitar a vida
            istringstream textureLine(line.substr(2));

            Vector3d texture;
            textureLine >> texture[U];
            textureLine >> texture[V];

            this->textures.push_back(texture);
        } else if (line.substr(0, 2) == "vn") {
            // stream de string para facilitar a vida
            istringstream normalLine(line.substr(2));

            Vector3d normal;
            normalLine >> normal[X];
            normalLine >> normal[Y];
            normalLine >> normal[Z];

            this->normals.push_back(normal);
        } else if (line[0] == '#') { /* Coment�rio: ignora */ }
        else { /* Ignora */ }
    }
}

void Mesh::draw() {
    for (Triangle t : elements) {
        glBegin(GL_TRIANGLES);
            for(Face face : t.faces) {
                Vector4d v  = vertices[face.vertex];
                Vector3d tx = textures[face.texture];
                Vector3d n  = normals[face.normal];

                glNormal3d(n[X], n[Y], n[Z]);

                glTexCoord2d(tx[X], tx[Y]);
                glVertex3d(v[X], v[Y], v[Z]);
            }
        glEnd();
    }
}
/*
void draw() {
    // Se o buffer estiver inicializado
    if (this->verticesVBO != 0) {
        glEnableVertexAttribArray(attribute_v_coord);
        glBindBuffer(GL_ARRAY_BUFFER, this->verticesVBO);
        glVertexAttribPointer(
            attribute_v_coord,  // attribute
            4,                  // number of elements per vertex, here (x,y,z,w)
            GL_FLOAT,           // the type of each element
            GL_FALSE,           // take our values as-is
            0,                  // no extra data between each position
            0                   // offset of first element
        );
    }

    if (this->vbo_normals != 0) {
        glEnableVertexAttribArray(attribute_v_normal);
        glBindBuffer(GL_ARRAY_BUFFER, this->vbo_normals);
        glVertexAttribPointer(
            attribute_v_normal, // attribute
            3,                  // number of elements per vertex, here (x,y,z)
            GL_FLOAT,           // the type of each element
            GL_FALSE,           // take our values as-is
            0,                  // no extra data between each position
            0                   // offset of first element
        );
    }

    /* Apply object's transformation matrix * /
    glUniformMatrix4fv(uniform_m, 1, GL_FALSE, glm::value_ptr(this->object2world));
    /* Transform normal vectors with transpose of inverse of upper left
    3x3 model matrix (ex-gl_NormalMatrix): * /
    glm::mat3 m_3x3_inv_transp = glm::transpose(glm::inverse(glm::mat3(this->object2world)));
    glUniformMatrix3fv(uniform_m_3x3_inv_transp, 1, GL_FALSE, glm::value_ptr(m_3x3_inv_transp));

    /* Push each element in buffer_vertices to the vertex shader * /
    if (this->ibo_elements != 0) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ibo_elements);
        int size;  glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
        glDrawElements(GL_TRIANGLES, size/sizeof(GLushort), GL_UNSIGNED_SHORT, 0);
    } else {
        glDrawArrays(GL_TRIANGLES, 0, this->vertices.size());
    }

    if (this->vbo_normals != 0)
        glDisableVertexAttribArray(attribute_v_normal);
    if (this->vbo_vertices != 0)
        glDisableVertexAttribArray(attribute_v_coord);
}
*/
