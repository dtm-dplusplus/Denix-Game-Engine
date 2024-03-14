#ifndef MODEL_H
#define MODEL_H

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <string>
#include <fstream>
#include <vector>

struct Model
{
    Model();
    Model(const std::string& _path);

    Model(const Model& _copy);
    Model& operator=(const Model& _assign);
    virtual ~Model();

    GLsizei vertex_count() const;
    GLuint vao_id();

private:
    struct Vertex
    {
        Vertex();

        glm::vec3 position;
        glm::vec2 texcoord;
        glm::vec3 normal;
    };

    struct Face
    {
        Vertex a;
        Vertex b;
        Vertex c;
    };

    std::vector<Face> m_faces;
    GLuint m_vaoid;
    GLuint m_vboid;
    bool m_dirty;

    void split_string_whitespace(const std::string& _input,
        std::vector<std::string>& _output);

    void split_string(const std::string& _input, char _splitter,
        std::vector<std::string>& _output);

};

#include <stdexcept>

inline Model::Model()
    : m_vboid(0)
    , m_vaoid(0)
    , m_dirty(false)
{ }

inline Model::Model(const std::string& _path)
    : m_vboid(0)
    , m_vaoid(0)
    , m_dirty(false)
{
    std::vector<glm::vec3> positions;
    std::vector<glm::vec2> tcs;
    std::vector<glm::vec3> normals;
    std::string currentline;

    std::ifstream file(_path.c_str());

    if (!file.is_open())
    {
        throw std::runtime_error("Failed to open model [" + _path + "]");
    }

    while (!file.eof())
    {
        std::getline(file, currentline);
        if (currentline.length() < 1) continue;

        std::vector<std::string> tokens;
        split_string_whitespace(currentline, tokens);
        if (tokens.size() < 1) continue;

        if (tokens.at(0) == "v" && tokens.size() >= 4)
        {
            glm::vec3 p(atof(tokens.at(1).c_str()),
                atof(tokens.at(2).c_str()),
                atof(tokens.at(3).c_str()));

            positions.push_back(p);
        }
        else if (tokens.at(0) == "vt" && tokens.size() >= 3)
        {
            glm::vec2 tc(atof(tokens.at(1).c_str()),
                1.0f - atof(tokens.at(2).c_str()));

            tcs.push_back(tc);
        }
        else if (tokens.at(0) == "vn" && tokens.size() >= 4)
        {
            glm::vec3 n(atof(tokens.at(1).c_str()),
                atof(tokens.at(2).c_str()),
                atof(tokens.at(3).c_str()));

            normals.push_back(n);
        }
        else if (tokens.at(0) == "f" && tokens.size() >= 4)
        {
            Face f;
            std::vector<std::string> sub;
            split_string(tokens.at(1), '/', sub);
            if (sub.size() >= 1) f.a.position = positions.at(atoi(sub.at(0).c_str()) - 1);
            if (sub.size() >= 2) f.a.texcoord = tcs.at(atoi(sub.at(1).c_str()) - 1);
            if (sub.size() >= 3) f.a.normal = normals.at(atoi(sub.at(2).c_str()) - 1);

            for (size_t ti = 2; ti + 1 < tokens.size(); ti++)
            {
                split_string(tokens.at(ti), '/', sub);
                if (sub.size() >= 1) f.b.position = positions.at(atoi(sub.at(0).c_str()) - 1);
                if (sub.size() >= 2) f.b.texcoord = tcs.at(atoi(sub.at(1).c_str()) - 1);
                if (sub.size() >= 3) f.b.normal = normals.at(atoi(sub.at(2).c_str()) - 1);

                split_string(tokens.at(ti + 1), '/', sub);
                if (sub.size() >= 1) f.c.position = positions.at(atoi(sub.at(0).c_str()) - 1);
                if (sub.size() >= 2) f.c.texcoord = tcs.at(atoi(sub.at(1).c_str()) - 1);
                if (sub.size() >= 3) f.c.normal = normals.at(atoi(sub.at(2).c_str()) - 1);

                m_faces.push_back(f);
                m_dirty = true;
            }
        }
    }
}

inline Model::~Model()
{
    if (m_vaoid)
    {
        glDeleteVertexArrays(1, &m_vaoid);
    }

    if (m_vboid)
    {
        glDeleteBuffers(1, &m_vboid);
    }
}

inline Model::Model(const Model& _copy)
    : m_vaoid(0)
    , m_vboid(0)
    , m_faces(_copy.m_faces)
    , m_dirty(true)
{ }

inline Model& Model::operator=(const Model& _assign)
{
    m_faces = _assign.m_faces;
    m_dirty = true;

    return *this;
}

inline void Model::split_string_whitespace(const std::string& _input,
    std::vector<std::string>& _output)
{
    std::string curr;

    _output.clear();

    for (size_t i = 0; i < _input.length(); i++)
    {
        if (_input.at(i) == ' ' ||
            _input.at(i) == '\r' ||
            _input.at(i) == '\n' ||
            _input.at(i) == '\t')
        {
            if (curr.length() > 0)
            {
                _output.push_back(curr);
                curr = "";
            }
        }
        else
        {
            curr += _input.at(i);
        }
    }

    if (curr.length() > 0)
    {
        _output.push_back(curr);
    }
}

inline void Model::split_string(const std::string& _input, char _splitter,
    std::vector<std::string>& _output)
{
    std::string curr;

    _output.clear();

    for (size_t i = 0; i < _input.length(); i++)
    {
        if (_input.at(i) == _splitter)
        {
            _output.push_back(curr);
            curr = "";
        }
        else
        {
            curr += _input.at(i);
        }
    }

    if (curr.length() > 0)
    {
        _output.push_back(curr);
    }
}

inline GLuint Model::vao_id()
{
    if (!m_faces.size())
    {
        throw std::runtime_error("Model is empty");
    }

    if (!m_vboid)
    {
        glGenBuffers(1, &m_vboid);

        if (!m_vboid)
        {
            throw std::runtime_error("Failed to generate vertex buffer");
        }
    }

    if (!m_vaoid)
    {
        glGenVertexArrays(1, &m_vaoid);

        if (!m_vaoid)
        {
            throw std::runtime_error("Failed to generate vertex array");
        }
    }

    if (m_dirty)
    {
        std::vector<GLfloat> data;

        for (size_t fi = 0; fi < m_faces.size(); ++fi)
        {
            data.push_back(m_faces[fi].a.position.x);
            data.push_back(m_faces[fi].a.position.y);
            data.push_back(m_faces[fi].a.position.z);
            data.push_back(m_faces[fi].a.texcoord.x);
            data.push_back(m_faces[fi].a.texcoord.y);
            data.push_back(m_faces[fi].a.normal.x);
            data.push_back(m_faces[fi].a.normal.y);
            data.push_back(m_faces[fi].a.normal.z);

            data.push_back(m_faces[fi].b.position.x);
            data.push_back(m_faces[fi].b.position.y);
            data.push_back(m_faces[fi].b.position.z);
            data.push_back(m_faces[fi].b.texcoord.x);
            data.push_back(m_faces[fi].b.texcoord.y);
            data.push_back(m_faces[fi].b.normal.x);
            data.push_back(m_faces[fi].b.normal.y);
            data.push_back(m_faces[fi].b.normal.z);

            data.push_back(m_faces[fi].c.position.x);
            data.push_back(m_faces[fi].c.position.y);
            data.push_back(m_faces[fi].c.position.z);
            data.push_back(m_faces[fi].c.texcoord.x);
            data.push_back(m_faces[fi].c.texcoord.y);
            data.push_back(m_faces[fi].c.normal.x);
            data.push_back(m_faces[fi].c.normal.y);
            data.push_back(m_faces[fi].c.normal.z);
        }

        glBindBuffer(GL_ARRAY_BUFFER, m_vboid);
        glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(data.at(0)), &data.at(0), GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glBindVertexArray(m_vaoid);
        glBindBuffer(GL_ARRAY_BUFFER, m_vboid);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
            8 * sizeof(data.at(0)), (void*)0);

        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,
            8 * sizeof(data.at(0)), (void*)(3 * sizeof(GLfloat)));

        glEnableVertexAttribArray(1);

        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE,
            8 * sizeof(data.at(0)), (void*)(5 * sizeof(GLfloat)));

        glEnableVertexAttribArray(2);

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        m_dirty = false;
    }

    return m_vaoid;
}

inline GLsizei Model::vertex_count() const
{
    return (GLsizei)m_faces.size() * 3;
}

inline Model::Vertex::Vertex()
    : position(0, 0, 0)
    , texcoord(0, 0)
    , normal(0, 0, 0)
{ }

#endif
